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

  p.setRenderHints(hints);

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
  p.end();
}
