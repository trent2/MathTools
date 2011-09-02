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

/*  file: --- BinomialDistPlotter.cpp --- */

#include <gsl/gsl_randist.h>  // use binomial distribution functions from the GNU Scientific Library
#include <gsl/gsl_cdf.h>

#include "BinomialDistPlotter.hpp"
#include "PlotterHelper.hpp"
#include "Calc.hpp"

BinomialDistPlotter::BinomialDistPlotter(QWidget *parent) : Plotter(parent), mN(20), mP(.5),
							    mKmin(0), mKmax(0), mPdf(true)
{
  setYMin(0);
  setYMax(1);
  setXMin(-1);
  setXMax(20);
}

void BinomialDistPlotter::paintIt(QPaintDevice *d, QPainter::RenderHints hints) const {
  QPainter p(d);
  paintIt(p, hints);
}

void BinomialDistPlotter::paintIt(QPainter &p, const QPainter::RenderHints &hints) const {
  p.setRenderHints(hints);
  QPaintDevice *d = p.device();

  PlotHelp::cs_params param = computeCSParameters(d);

  int xp, yp, xw;  // widget x-coordinate
  double y;

  plotGrid(p);

  // plot bars from i=0 to n
  for(int k=0; k<=mN; ++k) {
    // lower left corner of rectangle
    xp = calc::roundi((k - xMin()) * param.xstep)-calc::roundi(param.xstep/2);
    // width of rectangle
    xw = calc::roundi((k+1 - xMin()) * param.xstep) - xp - calc::roundi(param.xstep/2);

    if(xp-param.xstep<=xp && xp<=param.winwidth) {
      if(mPdf)
	y = gsl_ran_binomial_pdf(k, mP, mN);
      else
	y = gsl_cdf_binomial_P(k, mP, mN);

      yp = y*param.ystep;

      if(mPdf) {
	if(k>=mKmin && k <=mKmax)
	  p.fillRect(xp, param.winheight+calc::roundi(yMin()*param.ystep)-yp,
		     xw, yp, Qt::red);
      } else {
	if(mKmin == 0 && k == mKmax)
	  p.fillRect(xp, param.winheight+calc::roundi(yMin()*param.ystep)-yp,
		     xw, yp, Qt::red);
      }
      p.drawRect(xp, param.winheight+calc::roundi(yMin()*param.ystep)-yp,
		 xw, yp);
    }
  }
}
