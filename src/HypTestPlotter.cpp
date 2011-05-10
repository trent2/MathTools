#include <gsl/gsl_randist.h>

#include "HypTestPlotter.hpp"
#include "PlotterHelper.hpp"
#include "Calc.hpp"

const QString HypTestPlotter::regOfRejectionText = "H0's region of rejection";

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

  regTextSize = p.fontMetrics().size(Qt::TextSingleLine, regOfRejectionText);
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
	       Qt::AlignRight, regOfRejectionText);
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
    p.drawText(QRect(regionEnd, regTextSize), Qt::AlignLeft, regOfRejectionText);
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
