/*  MathTools 
    Copyright (C) 2010, 2011, 2012 Robert Spillner <Robert.Spillner@gmx.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*  file: --- ComplexPlotter.cpp --- */

#include <complex>
#include <iostream>
#include <QDebug>
#include <QtGui/QImage>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtCore/QElapsedTimer>

#include "ComplexPlotter.hpp"
#include "ComplexTab.hpp"
#include "Calc.hpp"

ComplexPlotter::ComplexPlotter(QWidget *parent) : Plotter(parent), mRayThickness(1),
						  mRayOpacity(1), mCircleThickness(1), mCircleOpacity(1),
						  mInfinityThreshold(1000), mImage(0), mRepaintEnabled(false),
						  mF(new MathFunction<std::complex<double> >),
						  finishCounter(0), mMutex(new QMutex), mStopwatch(new QElapsedTimer)
{
  for(int i=0; i<PAINT_THREAD_COUNT; ++i)
    paintThreads[i] = new ComplexPlotter::ImagePainterThread(this, i, mMutex);

  mLabel = new QLabel(this);
  mLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  QVBoxLayout *l = new QVBoxLayout(this);
  l->setContentsMargins(0,0,0,0);
  l->addWidget(mLabel);
  QColor(Qt::darkRed).getRgbF(&mUnitCircleColor[0], &mUnitCircleColor[1], &mUnitCircleColor[2]);
  setYMin(-2);
  setYMax(2);
  setXMin(-2);
  setXMax(2);
}

ComplexPlotter::~ComplexPlotter() {
  delete mF;
  delete mImage;
  delete mMutex;
  // mLabel and the box-layout created in the constructor is automatically destroyed my Qt
}

void ComplexPlotter::resizeImage() {
  if(mImage && mImage->size() != mLabel->size()) {
    delete mImage;
    mImage = new QImage(mLabel->size(), QImage::Format_RGB888);
  } else if(!mImage)
    mImage = new QImage(mLabel->size(), QImage::Format_RGB888);
}

void ComplexPlotter::setEnabledRepaintButton(bool b) {
  ComplexTab* ct = static_cast<ComplexTab*>(parentWidget());
  ct->getRepaintPushButton()->setEnabled(b);
}

void ComplexPlotter::doRepaint() {
  setEnabledRepaintButton(false);
  mRepaintEnabled = true;

  resizeImage();
  // call through to the superclass handler
  mStopwatch->start();
  for(int i=PAINT_THREAD_COUNT-1; i>=0; i--)
    paintThreads[i]->start();
}

void ComplexPlotter::paintEvent(QPaintEvent *e) {
  if(mRepaintEnabled) {
    qDebug() << "The image computation took " << mStopwatch->elapsed() << " milliseconds";
    mStopwatch->invalidate();
    resizeImage();
    Plotter::paintEvent(e);
    mLabel->setPixmap(QPixmap::fromImage(*mImage));
    setEnabledRepaintButton(true);
    mRepaintEnabled = false;
  }
}

// void ComplexPlotter::paintIt(QPaintDevice *d, QPainter::RenderHints hints) const {
// }

void ComplexPlotter::ImagePainterThread::run() {
  PlotHelp::cs_params param = mPlotter->computeCSParameters(mPlotter->mLabel);

  std::complex<double> f, z;

  double steoreographic_projection_ball_radius = .1;
  double ray_nums = 12;
  double log_base = 2;
  double xmin = mPlotter->xMin(),
    ymin = mPlotter->yMin(),
    infT = mPlotter->mInfinityThreshold,
    rayO  = mPlotter->mRayOpacity,
    cirO  = mPlotter->mCircleOpacity,
    rayT  = mPlotter->mRayThickness,
    cirT  = mPlotter->mCircleThickness;

  double h, s, v;  // hue, sat, val
  double m;        // modulus (Betrag)
  double lambda, mu, nu, dummy;

  // make a copy of the function object so we won't need thread locking
  // to slow us down
  MathFunction<std::complex<double> > nf(*mPlotter->mF);

  QColor pixelColor;
  // integer division
  int ystart = mThreadNumber*(param.winheight+1)/PAINT_THREAD_COUNT,
    yend = (mThreadNumber+1)*(param.winheight+1)/PAINT_THREAD_COUNT-1;

  if(nf.parseOk()) {
    for(int x=0; x <= param.winwidth; ++x)
      for(int y=ystart; y <= yend; ++y) {
	z.real(xmin + x/param.xstep);
	z.imag(ymin + (param.winheight-y)/param.ystep);
	f = nf(z);

	m = std::sqrt(f.real()*f.real()+f.imag()*f.imag());
	// log here is really ln, but that's unimportant
	double l = log(m)/log(log_base);

	h = std::atan2(f.imag(), f.real());
	s = (m>infT) ? 0 : 1;

	v = M_2_PI*std::atan(m/(2.0*steoreographic_projection_ball_radius));

	lambda = exp(-8000/rayT*pow(remainder(h,2*M_PI/ray_nums),2));
	h = fmod(h/(2*M_PI)+1,1);
	pixelColor.setHsvF(h, s, v);

	mu = fmod(modf(l, &dummy)+1, 1);
	nu = exp(-10000/cirT*pow(remainder(modf(l, &dummy), 0.5),2));

	qreal r[3];
	pixelColor.getRgbF(&r[0], &r[1], &r[2]);

	for(int i=0; i<3; ++i) {
	  r[i] *= (1-0.25*(1-mu*cirO));
	  r[i] = r[i]*(1-lambda*rayO) + lambda*rayO;
	  if(l>-.25 && l<.25) {
	    r[i] = r[i]*(1-nu*cirO) + nu*cirO*mPlotter->mUnitCircleColor[i];
	  }
	  else
	    r[i] *= (1-nu*cirO);
	}
	// mMux->lock();	
	setPixel(x, y, qRgb(r[0]*255, r[1]*255, r[2]*255));
	// mMux->unlock();
    }
  }
}

void ComplexPlotter::checkFinished() {
  if(++finishCounter == PAINT_THREAD_COUNT) {
    finishCounter = 0;
    update();
  }
}
