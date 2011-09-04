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

ComplexPlotter::ComplexPlotter(QWidget *parent) : Plotter(parent), mP(ComplexPlotter::Parameter()),
						  mRepaintEnabled(false),
						  finishCounter(0), mStopwatch(new QElapsedTimer)
{
  mP.mF = new MFC_t;
  paintThreads = new ImageRendererThread*[MAX_THREADS];  // there's a maximum of MAX_THREADS threads allowed

  for(int i=0; i<MAX_THREADS; ++i)
    paintThreads[i] = new ImageRendererThread(i);

  mLabel = new QLabel(this);
  mLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  QVBoxLayout *l = new QVBoxLayout(this);
  l->setContentsMargins(0,0,0,0);
  l->addWidget(mLabel);
  setYMin(-2);
  setYMax(2);
  setXMin(-2);
  setXMax(2);
}

ComplexPlotter::~ComplexPlotter() {
  delete mStopwatch;
  for(int i=0; i<MAX_THREADS; ++i)
    delete paintThreads[i];

  delete[] paintThreads;
  // mLabel and the box-layout created in the constructor is automatically destroyed my Qt
}

void ComplexPlotter::setNumThreads(int th) {
  // for(int i=0; i<mNThreads; ++i)
  //   delete paintThreads[i];

  // for(int i=0; i<th; ++i)
  //   paintThreads[i] = new ImageRendererThread(this, i);

  mP.mNThreads = th;
}

ImageRendererThread* ComplexPlotter::getImageRendererThread(int i) const {
  if(i<MAX_THREADS)
    return paintThreads[i];
  else
    return 0;
}

void ComplexPlotter::resizeImage() {
  if(mP.mImage && mP.mImage->size() != mLabel->size()) {
    delete mP.mImage;
    mP.mImage = new QImage(mLabel->size(), QImage::Format_RGB32);
  } else if(!mP.mImage)
    mP.mImage = new QImage(mLabel->size(), QImage::Format_RGB32);
}

void ComplexPlotter::setEnabledThreadStuff(bool b) {
  ComplexTab* ct = static_cast<ComplexTab*>(parentWidget());
  ct->getRepaintPushButton()->setEnabled(b);
  ct->getThreadsSpinBox()->setEnabled(b);
}

void ComplexPlotter::doRepaint() {
  setEnabledThreadStuff(false);
  mRepaintEnabled = true;

  resizeImage();
  // call through to the superclass handler
  mStopwatch->start();
  for(int i=mP.mNThreads-1; i>=0; i--)
    // paintThreads[i]->setPlotter(this);
    paintThreads[i]->render(mP, computeCSParameters(mLabel), xMin(), yMin());

  qDebug() << "started " << mP.mNThreads << " threads.";
}

void ComplexPlotter::paintEvent(QPaintEvent *e) {
  if(mRepaintEnabled) {
    qDebug() << "The image computation took " << mStopwatch->elapsed() << " milliseconds";
    mStopwatch->invalidate();
    resizeImage();
    Plotter::paintEvent(e);
    mLabel->setPixmap(QPixmap::fromImage(*mP.mImage));
    setEnabledThreadStuff(true);
    mRepaintEnabled = false;
  }
}

// void ComplexPlotter::paintIt(QPaintDevice *d, QPainter::RenderHints hints) const {
// }

/***************************************
 *                                     *
 *        ImageRendererThread          *
 *                                     *
 ***************************************/

ImageRendererThread::~ImageRendererThread() {
  mMutex.lock();
  mFinish = true;
  condition.wakeOne();
  mMutex.unlock();
  wait();
}

void ImageRendererThread::render(const ComplexPlotter::Parameter &p,
				 const PlotHelp::cs_params &csp, int xmin, int ymin) {
  QMutexLocker locker(&mMutex);  // lock the whole method

  mP = p;
  mXmin = xmin;
  mYmin = ymin;
  mCSP = csp;
  if(!isRunning()) {
    qDebug() << "Thread " << mThreadNumber << " about to start";
    start();
  }
  else {
    qDebug() << "Thread " << mThreadNumber << " about to wake";
    condition.wakeOne();
  }
}

void ImageRendererThread::run() {
  std::complex<double> f, z;
  double steoreographic_projection_ball_radius = .1;
  double ray_nums = 12;
  double log_base = 2;
  QColor pixelColor;
  double h, s, v;  // hue, sat, val
  double m;        // modulus (Betrag)
  double lambda, mu, nu, dummy;
  QRgb *sl;

  forever {
    mMutex.lock();
    PlotHelp::cs_params param = mCSP;
    double xmin = mXmin,
      ymin = mYmin,
      infT = mP.mInfinityThreshold,
      rayO  = mP.mRayOpacity,
      cirO  = mP.mCircleOpacity,
      rayT  = mP.mRayThickness,
      cirT  = mP.mCircleThickness;

    // make a copy of the function object so we won't need thread locking
    // to slow us down
    MFC_t nf(*mP.mF);
    QImage *mImage = mP.mImage;

    // integer division
    int ystart = mThreadNumber*(param.winheight+1)/mP.mNThreads,
      yend = (mThreadNumber+1)*(param.winheight+1)/mP.mNThreads-1;
    mMutex.unlock();

    qDebug() << "Thread " << mThreadNumber << " from " << ystart << " to " << yend;

    if(nf.parseOk()) {
      for(int y=ystart; y <= yend; ++y) {
	if(mFinish)
	  return;
	sl = reinterpret_cast<QRgb*>(mImage->scanLine(y));
	for(int x=0; x <= param.winwidth; ++x) {
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
	      r[i] = r[i]*(1-nu*cirO) + nu*cirO*mP.mUnitCircleColor[i];
	    }
	    else
	      r[i] *= (1-nu*cirO);
	  }

	  /* Note: This is a dangerous play! setPixel is NOT thread-safe,
	   * yet the QImage-instance (mImage) is shared and written to by different
	   * threads which are all computing the image.
	   *
	   * It SEEMS to work --- probably, because each thread writes to DIFFERENT
	   * addresses of the image (i.e. each thread computes unique parts of the image).
	   * However, it is not guaranteed that one day this might somehow change in the
	   * Qt-implementation.
	   *
	   * Unfortunately, locking the resource with a mutex results in a drastic
	   * increase in computation time so we refrain from it at this point.
	   */
	  sl[x] = qRgb(r[0]*255, r[1]*255, r[2]*255);
	}
      }
    }
    emit rendered();
    mMutex.lock();
    qDebug() << "Thread " << mThreadNumber << " about to sleep";
    condition.wait(&mMutex);
    qDebug() << "Thread " << mThreadNumber << " woken, continuing my work";
    mMutex.unlock();
  }
}

void ComplexPlotter::checkFinished() {
  if(++finishCounter == mP.mNThreads) {
    finishCounter = 0;
    update();
  }
}
