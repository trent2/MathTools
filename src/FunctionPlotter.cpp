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

/*  file: --- FunctionPlotter.cpp --- */

#include <vector>

#include <QtGui/QFrame>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

#include "FunctionPlotter.hpp"
#include "MathFunction.hpp"
#include "Calc.hpp"

class FunctionPlotter_Implementation {
private:
  FunctionPlotter_Implementation(FunctionPlotter *pSrc);
  ~FunctionPlotter_Implementation();

  friend class FunctionPlotter;
  Plotter *src;

  std::vector<MathFunction<double> > mfunc;

  // flag to determine whether to draw vertical lines
  bool verticalCorrection;
};

FunctionPlotter_Implementation::FunctionPlotter_Implementation(FunctionPlotter *pSrc) :
    src(pSrc), verticalCorrection(true) {
  src->setCursor(Qt::OpenHandCursor);
  mfunc.push_back(std::string(""));
  mfunc.push_back(std::string(""));
  mfunc.push_back(std::string(""));
  mfunc.push_back(std::string(""));
}

FunctionPlotter_Implementation::~FunctionPlotter_Implementation() {
  mfunc.clear();
}

FunctionPlotter::FunctionPlotter(QWidget *parent) :
  Plotter(parent), impl_fp(new FunctionPlotter_Implementation(this)) {
  setXMin(-10);
  setXMax(10);
  setYMin(-10);
  setYMax(10);
}

FunctionPlotter::~FunctionPlotter() {
  delete impl_fp;
}

MathFunction<double>& FunctionPlotter::getFunction(int index) {
  return impl_fp->mfunc[index];
}

void FunctionPlotter::paintIt(QPaintDevice *d, QPainter::RenderHints hints) const {
  QPainter p(d);

  p.setRenderHints(hints);

  bool nextIsMove;
  double y, yold, yplot, yplotold;

  qreal res_factor = PlotHelp::getResFactor(p.device(), this);
  PlotHelp::cs_params param = computeCSParameters(d);

  // const double xustep = 1.0/param.xstep;


  //  p.setRenderHint(QPainter::Antialiasing);

  //  p.setPen(QPen(Qt::black, 1));

  plotGrid(p);

  // draw functions
  for(int i=0; i<4; ++i)
    if(impl_fp->mfunc[i].parseOk()) {  // function term is parsable hence evaluable

      QPainterPath pathstroke;
      nextIsMove = true;

      p.setPen(QPen(impl_fp->mfunc[i].getColor(), 2*res_factor));

      for(int x=0; x<=param.winwidth; ++x) {
	// compute value of functions
	y = impl_fp->mfunc[i](xMin()+x/param.xstep);
	if(!x)
	  yold = y;

	yplot = param.winheight-calc::roundi((y-yMin())*param.ystep);
	yplotold = param.winheight-calc::roundi((yold-yMin())*param.ystep);

	// don't draw lines crossing the screen vertically; also don't draw to infinity or nan
	if((impl_fp->verticalCorrection && (yplotold<0 || yplotold>param.winheight) && (yplot<0 || yplot>param.winheight)) ||
	   std::isnan(y) || std::isinf(y))
	  nextIsMove = true;
	else if(yplot<0)
	  yplot = 0;
	else if(yplot>param.winheight)
	  yplot = param.winheight;

	if(nextIsMove) {
	  pathstroke.moveTo(x, yplot);
	  nextIsMove = false;
	} else {
	  pathstroke.lineTo(x, yplot);
	}
	yold = y;
      }
      p.drawPath(pathstroke);
    }
  p.end();
}

void FunctionPlotter::setVerticalCorrection(bool v) {
  impl_fp->verticalCorrection = v;
}
