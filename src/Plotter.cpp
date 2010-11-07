#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include <QtGui/QFrame>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

#include "Plotter.hpp"
#include "MathFunction.hpp"
#include "Calc.hpp"
#include "PlotterHelper.hpp"

const int Plotter::tickPixelDistForAuto = 50;
const int Plotter::descAutoPixelDist = 100;
const double Plotter::zoomFactor = 1.2;


Plotter::Plotter(QWidget *parent) : QFrame(parent),
				    compAutoXTicks(false), compAutoYTicks(false), drawGridX(false), drawGridY(false),
				    xmin(-10), xmax(10), ymin(-10), ymax(10), xticks(1), yticks(1),
				    verticalCorrection(true), leftPressed(false) {
  setCursor(Qt::OpenHandCursor);
  mfunc.push_back(std::string(""));
  mfunc.push_back(std::string(""));
  mfunc.push_back(std::string(""));
  mfunc.push_back(std::string(""));
}

Plotter::~Plotter() {
  mfunc.clear();
}

void Plotter::update() {
  QWidget::update();
}

MathFunction& Plotter::getFunction(int index) {
  return mfunc[index];
}

void Plotter::paintEvent(QPaintEvent *) {
  // do non-const operations
  computeAutoTicks();
  screen_params = computeCSParameters(this);
  for(int i=0; i<4; ++i)
    if(mfunc[i].parseOk())
      mfunc[i].setUStepsize(1.0/screen_params.xstep);

  paintIt(this);
}

void Plotter::paintIt(QPaintDevice *d, QPainter::RenderHints hints) const {
  QPainter p(d);

  p.setRenderHints(hints);

  bool nextIsMove;
  double y, yold, yplot, yplotold;
  qreal res_factor = PlotHelp::getResFactor(p.device(), this);

  cs_params param;
  if(d != this)
    param = computeCSParameters(d);
  else
    param = screen_params;

  // const double xustep = 1.0/param.xstep;


  //  p.setRenderHint(QPainter::Antialiasing);

  //  p.setPen(QPen(Qt::black, 1));

  plotGrid(p, param, res_factor);

  // draw functions
  for(int i=0; i<4; ++i)
    if(mfunc[i].parseOk()) {  // function term is parsable hence evaluable

      QPainterPath pathstroke;
      nextIsMove = true;

      p.setPen(QPen(mfunc[i].getColor(), 2*res_factor));

      for(int x=0; x<=param.winwidth; ++x) {
	// compute value of functions
	y = mfunc[i](xmin+x/param.xstep);
	if(!x)
	  yold = y;

	yplot = param.winheight-calc::roundi((y-ymin)*param.ystep);
	yplotold = param.winheight-calc::roundi((yold-ymin)*param.ystep);

	// don't draw lines crossing the screen vertically; also don't draw to infinity or nan
	if((verticalCorrection && (yplotold<0 || yplotold>param.winheight) && (yplot<0 || yplot>param.winheight)) ||
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

void Plotter::plotGrid(QPainter &p, const cs_params &param, qreal res_factor) const {
  double tick;
  QSize textSizeMax;
  QFont myFont;
  // QPen myPen;
  QString number;
  std::vector<QString> axisLabels;
  std::ostringstream numberstream;

  myFont.setPointSize(12);
  p.setFont(myFont);

  const double
    xtop = (drawGridY ? param.winwidth : calc::roundi(-xmin*param.xstep)+3),
    xbot = (drawGridY ? 0 : calc::roundi(-xmin*param.xstep)-3),
    ytop = (drawGridX ? 0 : param.winheight-calc::roundi(-ymin*param.ystep)-3),
    ybot = (drawGridX ? param.winheight : param.winheight-calc::roundi(-ymin*param.ystep)+3);

  // is there a y-axis?
  if((xbot>=0 && xtop<=param.winwidth) || drawGridY) {
    tick = std::ceil(ymin/yticks)*yticks;

    if(drawGridY)
      p.setPen(QPen(Qt::black, 1*res_factor, Qt::DotLine));
    // draw tick marks/grid lines
    while(tick <= ymax) {
      p.drawLine(std::max(0.0,xbot), param.winheight-calc::roundi((tick-ymin)*param.ystep),
		 std::min((double)param.winwidth, xtop), param.winheight-calc::roundi((tick-ymin)*param.ystep));
      tick += yticks;
    }

    tick = std::ceil(ymin/param.descDistY)*param.descDistY;
    // draw numbers
    // determin maximum size needed for rendering axis-labels
    QFontMetrics fm = p.fontMetrics();
    PlotHelp::createAxisLabels(axisLabels, tick, std::floor(ymax/param.descDistY)*param.descDistY, param.descDistY,
			       &textSizeMax, &fm, false);

    for(uint i=0; i<axisLabels.size(); ++i) {
      p.drawText(QRect(QPoint(calc::roundi(-xmin*param.xstep)+5, param.winheight-calc::roundi((tick-ymin)*param.ystep)-textSizeMax.height()/2),
		       textSizeMax), Qt::AlignRight, axisLabels[i]);
      tick += param.descDistY;
    }
    p.setPen(QPen(Qt::black, 1*res_factor));

    // draw axis
    if(xbot>=0 && xtop<=param.winwidth)
       p.drawLine(calc::roundi(-xmin*param.xstep), 0, calc::roundi(-xmin*param.xstep), param.winheight);
  }

  // is there an x-axis?
  if((ybot>=0 && ytop <=param.winheight) || drawGridX) {
    tick = std::ceil(xmin/xticks)*xticks;

    if(drawGridX)
      p.setPen(QPen(Qt::black, 1*res_factor, Qt::DotLine));
    // draw tick marks/grid lines
    while(tick <= xmax) {
      p.drawLine(calc::roundi((tick-xmin)*param.xstep), std::min((double)param.winheight, ybot),
		 calc::roundi((tick-xmin)*param.xstep), std::max(0.0,ytop));
      tick += xticks;
    }

    tick = std::ceil(xmin/param.descDistX)*param.descDistX;
    QFontMetrics fm = p.fontMetrics();
    axisLabels.clear();
    PlotHelp::createAxisLabels(axisLabels, tick, std::floor(xmax/param.descDistX)*param.descDistX, param.descDistX,
			       &textSizeMax, &fm, true);
    for(uint i=0; i<axisLabels.size(); ++i) {
      p.drawText(QRect(QPoint(calc::roundi((tick-xmin)*param.xstep)-textSizeMax.width()/2, param.winheight-calc::roundi(-ymin*param.ystep)+5),
		       textSizeMax), Qt::AlignRight, axisLabels[i]);
      tick += param.descDistX;
    }

    p.setPen(QPen(Qt::black, 1*res_factor));
    // draw axis

    if(ybot>=0 && ytop <= param.winheight)
      p.drawLine(0, param.winheight-calc::roundi(-ymin*param.ystep), param.winwidth, param.winheight-calc::roundi(-ymin*param.ystep));
  }
}

/** This methods computes

    - xticks, yticks (delta for tick marks in real cs) if autoticks is enabled
    - xstep, ystep (number of device units per cs unit)
    - descDistX, descDistY (distance between numbers drawn on the display)
 */
Plotter::cs_params Plotter::computeCSParameters(const QPaintDevice *paintDev) const {
  cs_params ret;
  ret.winwidth = paintDev->width()-1;
  ret.winheight = paintDev->height()-1;

  ret.xstep = ret.winwidth/(xmax-xmin);
  ret.ystep = ret.winheight/(ymax-ymin);

  // compute distances between numbers labels on axes
  ret.descDistX = std::min(xticks, std::ceil(((xmax-xmin)*((double)descAutoPixelDist)/ret.winwidth)/xticks)*xticks);
  ret.descDistY = std::min(yticks, std::ceil(((ymax-ymin)*((double)descAutoPixelDist)/ret.winheight)/yticks)*yticks);

  return ret;
}

void Plotter::computeAutoTicks() {
  if(compAutoXTicks) {
    xticks = normalizeTickValue((xmax-xmin)*((double)tickPixelDistForAuto)/width());
    emit newXTicks(xticks);
  }
  if(compAutoYTicks) {
    yticks = normalizeTickValue((ymax-ymin)*((double)tickPixelDistForAuto)/height());
    emit newYTicks(yticks);
  }
}

double Plotter::normalizeTickValue(double tick) const {
  // only use positive tick value
  if(tick>0) {
    if(tick>0.5)
      tick = calc::round(tick*2)/2;
    else {
      int power = -std::floor(std::log(tick)/std::log(10));
      if(power <= 4)
	tick = calc::roundi(std::pow(10,power)*tick)*std::pow(10.0,-power);
      else
	tick = 1e-4;
    }
  }
  return tick;
}

void Plotter::setCenter(double x, double y) {
  // center of cs
  double mx = (xmax+xmin)/2,
    my = (ymax+ymin)/2;
  
  xmin += (x-mx);
  xmax += (x-mx);
  ymin += (y-my);
  ymax += (y-my);

  emit newXMin(xmin);
  emit newXMax(xmax);
  emit newYMin(ymin);
  emit newYMax(ymax);
}

void Plotter::resizeAxes(double dx, double dy) {
  xmin = old_vals.xmin+dx/2;
  xmax = old_vals.xmax-dx/2;
  ymin = old_vals.ymin+dy/2;
  ymax = old_vals.ymax-dy/2;

  emit newXMin(xmin);
  emit newXMax(xmax);
  emit newYMin(ymin);
  emit newYMax(ymax);
}

void Plotter::zoomToCenter(int wx, int wy, double f) {
  int dw = wx-screen_params.winwidth/2;
  double tw = screen_params.xstep;
  int dh = screen_params.winheight/2-wy;
  double th = screen_params.ystep;

  // shift (x,y) to center
  xmin += dw/tw;
  xmax += dw/tw;
  ymin += dh/th;
  ymax += dh/th;  

  tw = (xmax-xmin)*((f-1)/(2*f));
  th = (ymax-ymin)*((f-1)/(2*f));

  // zoom
  xmin += tw;
  xmax -= tw;
  ymin += th;
  ymax -= th;

  // shift (x,y) back
  tw = screen_params.winwidth/(xmax-xmin);
  th = screen_params.winheight/(ymax-ymin);

  xmin -= dw/tw;
  xmax -= dw/tw;
  ymin -= dh/th;
  ymax -= dh/th;

  emit newXMin(xmin);
  emit newXMax(xmax);
  emit newYMin(ymin);
  emit newYMax(ymax);
  setCenter((xmax+xmin)/2, (ymax+ymin)/2);
}

void Plotter::wheelEvent(QWheelEvent *e) {
  zoomToCenter(e->x(), e->y(), e->delta() < 0 ? zoomFactor : 1.0/zoomFactor);
}

void Plotter::mousePressEvent(QMouseEvent *e) {
  leftPressed = false;
  key_mod = e->modifiers();

  if(e->button() == Qt::LeftButton) {
    leftPressed = true;
    setCursor(Qt::ClosedHandCursor);
    old_vals.anchorMX = (xmax+xmin)/2;
    old_vals.anchorMY = (ymax+ymin)/2;
    old_vals.anchorWX = e->x();
    old_vals.anchorWY = e->y();

    if(key_mod == Qt::ShiftModifier) {
      setCursor(Qt::SizeAllCursor);
      old_vals.xmin = xmin;
      old_vals.xmax = xmax;
      old_vals.ymin = ymin;
      old_vals.ymax = ymax;
    }
  }
}

void Plotter::mouseReleaseEvent(QMouseEvent*) {
  setCursor(Qt::OpenHandCursor);
}

void Plotter::mouseMoveEvent(QMouseEvent *e) {
  int dx = e->x() - old_vals.anchorWX, dy = old_vals.anchorWY - e->y();


  if(leftPressed)
    switch(key_mod) {
    case Qt::NoModifier :
      setCenter(old_vals.anchorMX - dx/screen_params.xstep, old_vals.anchorMY - dy/screen_params.ystep); break;
    case Qt::ShiftModifier :
      resizeAxes(dx/screen_params.xstep, dy/screen_params.ystep);break;
    default: break;
    }
}
