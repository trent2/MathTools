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

/*  file: --- HypTestPlotter.cpp --- */

#include <gsl/gsl_randist.h>

#include "HypTestPlotter.hpp"
#include "PlotterHelper.hpp"
#include "Calc.hpp"

HypTestPlotter::HypTestPlotter(QWidget *parent, TestType tt) : BinomialDistPlotter(parent), testtype(tt),
							       testresult(0) { }

void HypTestPlotter::paintIt(QPaintDevice *d, QPainter::RenderHints hints) const {
  // call overwritten method from superclass
  QPainter p(d);
  QPoint regionEnd;
  QSize regTextSize, resSize;

  p.setRenderHints(hints);
  PlotHelp::cs_params param = computeCSParameters(d);
  plotGrid(p);

  regTextSize = p.fontMetrics().size(Qt::TextSingleLine, tr("H0's region of rejection"));
  resSize = p.fontMetrics().size(Qt::TextSingleLine, "X");

  const QPen ppen = p.pen();
  QPen linePen;
  linePen.setWidth(3);
  linePen.setColor(Qt::darkGreen);
  // plot bars from i=0 to n
  if(testtype & (left | both)) {
    for(int k=0; k<=klow; ++k) {
      blotBar(k, p, param);
    }

    // draw line to indicate the region of rejection
    regionEnd.setX(calc::roundi((klow-xMin())*param.xstep));
    regionEnd.setY(param.winheight+calc::roundi(yMin()*param.ystep)+30);
    p.setPen(linePen);
    p.drawLine(QPoint(calc::roundi(-xMin()*param.xstep)-calc::roundi(param.xstep/2),
		      param.winheight+calc::roundi(yMin()*param.ystep)+30),
	       regionEnd);
    p.drawText(QRect(QPoint(regionEnd.x()-regTextSize.width(), regionEnd.y()+10), regTextSize),
	       Qt::AlignRight, tr("H0's region of rejection"));
    p.setPen(ppen);
  }
  
  if(testtype & (right | both)) {
    for(int k=khigh; k<=mN; ++k) {
      blotBar(k, p, param);
    }
    // draw line to indicate the region of rejection
    regionEnd.setX(calc::roundi((khigh-xMin())*param.xstep));
    regionEnd.setY(param.winheight+calc::roundi(yMin()*param.ystep)+30);
    p.setPen(linePen);
    p.drawLine(QPoint(calc::roundi((mN-xMin())*param.xstep)+calc::roundi(param.xstep/2), param.winheight+calc::roundi(yMin()*param.ystep)+30),
	       regionEnd);
    regionEnd += QPoint(0,10);
    p.drawText(QRect(regionEnd, regTextSize), Qt::AlignLeft, tr("H0's region of rejection"));
  }

  linePen.setColor(Qt::red);
  linePen.setWidth(2);
  p.setPen(linePen);
  QPoint resultPoint = QPoint(calc::roundi((testresult-xMin())*param.xstep),
			      param.winheight+calc::roundi(yMin()*param.ystep));
  p.drawLine(resultPoint, resultPoint+QPoint(0,20));
  p.drawText(QRect(resultPoint + QPoint(-resSize.width()/2,30), resSize), Qt::AlignCenter, "X");

  linePen.setColor(Qt::black);
  p.setPen(linePen);

  BinomialDistPlotter::paintIt(p, hints);
}

void HypTestPlotter::blotBar(int k, QPainter &p, const PlotHelp::cs_params &param) const {
  int xp, yp, xw;  // widget x-coordinate
  double y;
    // lower left corner of rectangle
    xp = calc::roundi((k - xMin()) * param.xstep)-calc::roundi(param.xstep/2);
    // width of rectangle
    xw = calc::roundi((k+1 - xMin()) * param.xstep) - xp - calc::roundi(param.xstep/2);

  if(xp-param.xstep<=xp && xp<=param.winwidth) {
    y = gsl_ran_binomial_pdf(k, mP, mN);

    yp = y*param.ystep;
    p.fillRect(xp, param.winheight+calc::roundi(yMin()*param.ystep)-yp,
	       xw, yp, Qt::green);
  }
}
