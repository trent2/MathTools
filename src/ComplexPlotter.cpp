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
#include <QtGui/QImage>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

#include "ComplexPlotter.hpp"
#include "Calc.hpp"

ComplexPlotter::ComplexPlotter(QWidget *parent) : Plotter(parent), mRayThickness(1),
						  mRayOpacity(1), mCircleThickness(1), mCircleOpacity(1),
						  mInfinityThreshold(1000), mImage(0), mRepaintEnabled(false)
{
  mF = new MathFunction<std::complex<double> >;
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
  // mLabel and the box-layout created in the constructor is automatically destroyed my Qt
}

void ComplexPlotter::doARepaint() {
  mRepaintEnabled = true;
  update();
}

void ComplexPlotter::paintEvent(QPaintEvent *e) {
  if(mRepaintEnabled) {
    if(mImage != 0 && mImage->size() != mLabel->size()) {
      delete mImage;
      mImage = new QImage(mLabel->size(), QImage::Format_RGB888);
    } else if(mImage == 0)
      mImage = new QImage(mLabel->size(), QImage::Format_RGB888);

    // call through to the superclass handler
    Plotter::paintEvent(e);
    mRepaintEnabled = false;
  }
}

void ComplexPlotter::paintIt(QPaintDevice *d, QPainter::RenderHints hints) const {
  PlotHelp::cs_params param = computeCSParameters(mLabel);

  std::complex<double> f, z;

  double projection_radius = .1;
  double ray_nums = 12;
  double log_base = 2;

  double h, s, v;  // hue, sat, val
  double m;        // modulus (Betrag)
  double lambda, mu, nu, dummy;
  QColor pixelColor;

  if(mF->parseOk()) {
    for(int x=0; x <= param.winwidth; ++x)
      for(int y=0; y <= param.winheight; ++y) {
	z.real(xMin() + x/param.xstep);
	z.imag(yMin() + (param.winheight-y)/param.ystep);
	f = (*mF)(z);

	m = std::sqrt(f.real()*f.real()+f.imag()*f.imag());
	// log here is really ln, but that's unimportant
	double l = log(m)/log(log_base);

	h = std::atan2(f.imag(), f.real());
	s = (m>mInfinityThreshold) ? 0 : 1;

	v = M_2_PI*std::atan(m/(2.0*projection_radius));


	// lambda = exp(-8000*(1-mRayThickness)*pow(remainder(h,2*M_PI/ray_nums),2));
	lambda = exp(-8000/mRayThickness*pow(remainder(h,2*M_PI/ray_nums),2));
	h = fmod(h/(2*M_PI)+1,1);
	pixelColor.setHsvF(h, s, v);

	mu = fmod(modf(l, &dummy)+1, 1);
	// nu = exp(-10000*(1-mCircleThickness)*pow(remainder(modf(log(m)/log(log_base), &dummy), 0.5),2));
	nu = exp(-10000/mCircleThickness*pow(remainder(modf(l, &dummy), 0.5),2));

	qreal r[3];
	pixelColor.getRgbF(&r[0], &r[1], &r[2]);

	for(int i=0; i<3; ++i) {
	  r[i] *= (1-0.25*(1-mu*mCircleOpacity));
	  r[i] = r[i]*(1-lambda*mRayOpacity) + lambda*mRayOpacity;
	  if(l>-.25 && l<.25)
	    r[i] = r[i]*(1-nu*mCircleOpacity) + nu*mCircleOpacity*mUnitCircleColor[i];
	  else
	    r[i] *= (1-nu*mCircleOpacity);
	}

	mImage->setPixel(x, y, qRgb(r[0]*255, r[1]*255, r[2]*255));
    }
    mLabel->setPixmap(QPixmap::fromImage(*mImage));
  }
}
