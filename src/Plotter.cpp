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

class Plotter_Implementation {
private:
  Plotter_Implementation(Plotter *pSrc);
  ~Plotter_Implementation();

  friend class Plotter;
  Plotter *src;

  // compute x- and y-ticks automatically
  bool compAutoXTicks, compAutoYTicks;
  // draw grid
  bool drawGridX, drawGridY;

  static const int tickPixelDistForAuto;
  static const int descAutoPixelDist;
  static const double zoomFactor;

  // minimum and maximum of real view window
  double xmin, xmax, ymin, ymax;
  // tick-lenght for axis ticks
  double xticks, yticks;

  // flag to determine whether to draw vertical lines
  bool verticalCorrection;

  // for mouse movement of the graph
  bool leftPressed;
  // modifiers hit when mouse was pressed
  unsigned int key_mod;

  // old values for tracking resize and move operations
  struct old_vals_t {
    double xmin, xmax, ymin, ymax;
    // mean(xmax,xmin), mean(ymax,ymin)
    double anchorMX, anchorMY;
    // position of mouse move start (in pixel coords)
    int anchorWX, anchorWY;
  } old_vals;

  struct cs_params {
    int winwidth, winheight;
    // pixels per unit
    double xstep, ystep;
    // distance between number label on axes
    double descDistX, descDistY;
  } screen_params;

  std::vector<MathFunction> mfunc;

  void plotGrid(QPainter&, const cs_params&, qreal) const;
  void computeAutoTicks();
  cs_params computeCSParameters(const QPaintDevice*) const;
  double normalizeTickValue(double) const;

  // set (x,y) as the center of the cs
  void setCenter(double x, double y);

  // change axes size by dx, dy
  void resizeAxes(double dx, double dy);

  // f>1 indicates to zoom in, f<1 zooms out
  void zoomToCenter(int, int, double f);
};

const int Plotter_Implementation::tickPixelDistForAuto = 50;
const int Plotter_Implementation::descAutoPixelDist = 100;
const double Plotter_Implementation::zoomFactor = 1.2;

Plotter_Implementation::Plotter_Implementation(Plotter *pSrc) : src(pSrc),
    compAutoXTicks(false), compAutoYTicks(false), drawGridX(false), drawGridY(false),
    xmin(-10), xmax(10), ymin(-10), ymax(10), xticks(1), yticks(1),
    verticalCorrection(true), leftPressed(false) {
  src->setCursor(Qt::OpenHandCursor);
  mfunc.push_back(std::string(""));
  mfunc.push_back(std::string(""));
  mfunc.push_back(std::string(""));
  mfunc.push_back(std::string(""));
}



Plotter_Implementation::~Plotter_Implementation() {
  mfunc.clear();
}

Plotter::Plotter(QWidget *parent) : QFrame(parent), impl(new Plotter_Implementation(this)) { }

Plotter::~Plotter() {
  delete impl;
}

void Plotter::setXMin(double xm) {
  impl->xmin = xm; newXMin(xm);
}

void Plotter::setXMax(double xm) {
  impl->xmax = xm; newXMax(xm);
}

void Plotter::setYMin(double ym) {
  impl->ymin = ym; newYMin(ym);
}

void Plotter::setYMax(double ym) {
  impl->ymax = ym; newYMax(ym);
}

void Plotter::setXTicks(double xt) {
  impl->xticks = xt; newXTicks(xt);
}

void Plotter::setYTicks(double yt) {
  impl->yticks = yt; newYTicks(yt);
}

void Plotter::setCompAutoXTicks(bool b) {
  impl->compAutoXTicks = b;
}

void Plotter::setCompAutoYTicks(bool b) {
  impl->compAutoYTicks = b;
}

void Plotter::setDrawGridX(bool b) {
  impl->drawGridX = b;
}

void Plotter::setDrawGridY(bool b) {
  impl->drawGridY = b;
}

void Plotter::update() {
  QWidget::update();
}

MathFunction& Plotter::getFunction(int index) {
  return impl->mfunc[index];
}

void Plotter::paintEvent(QPaintEvent *) {
  // do non-const operations
  impl->computeAutoTicks();
  impl->screen_params = impl->computeCSParameters(this);
  for(int i=0; i<4; ++i)
    if(impl->mfunc[i].parseOk())
      impl->mfunc[i].setUStepsize(1.0/impl->screen_params.xstep);

  paintIt(this);
}

void Plotter::paintIt(QPaintDevice *d, QPainter::RenderHints hints) const {
  QPainter p(d);

  p.setRenderHints(hints);

  bool nextIsMove;
  double y, yold, yplot, yplotold;
  qreal res_factor = PlotHelp::getResFactor(p.device(), this);

  Plotter_Implementation::cs_params param;
  if(d != this)
    param = impl->computeCSParameters(d);
  else
    param = impl->screen_params;

  // const double xustep = 1.0/param.xstep;


  //  p.setRenderHint(QPainter::Antialiasing);

  //  p.setPen(QPen(Qt::black, 1));

  impl->plotGrid(p, param, res_factor);

  // draw functions
  for(int i=0; i<4; ++i)
    if(impl->mfunc[i].parseOk()) {  // function term is parsable hence evaluable

      QPainterPath pathstroke;
      nextIsMove = true;

      p.setPen(QPen(impl->mfunc[i].getColor(), 2*res_factor));

      for(int x=0; x<=param.winwidth; ++x) {
	// compute value of functions
	y = impl->mfunc[i](impl->xmin+x/param.xstep);
	if(!x)
	  yold = y;

	yplot = param.winheight-calc::roundi((y-impl->ymin)*param.ystep);
	yplotold = param.winheight-calc::roundi((yold-impl->ymin)*param.ystep);

	// don't draw lines crossing the screen vertically; also don't draw to infinity or nan
	if((impl->verticalCorrection && (yplotold<0 || yplotold>param.winheight) && (yplot<0 || yplot>param.winheight)) ||
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

void Plotter_Implementation::plotGrid(QPainter &p, const cs_params &param, qreal res_factor) const {
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
Plotter_Implementation::cs_params Plotter_Implementation::computeCSParameters(const QPaintDevice *paintDev) const {
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

void Plotter_Implementation::computeAutoTicks() {
  if(compAutoXTicks) {
    xticks = normalizeTickValue((xmax-xmin)*((double)tickPixelDistForAuto)/src->width());
    emit src->newXTicks(xticks);
  }
  if(compAutoYTicks) {
    yticks = normalizeTickValue((ymax-ymin)*((double)tickPixelDistForAuto)/src->height());
    emit src->newYTicks(yticks);
  }
}

double Plotter_Implementation::normalizeTickValue(double tick) const {
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

void Plotter_Implementation::setCenter(double x, double y) {
  // center of cs
  double mx = (xmax+xmin)/2,
    my = (ymax+ymin)/2;
  
  xmin += (x-mx);
  xmax += (x-mx);
  ymin += (y-my);
  ymax += (y-my);

  emit src->newXMin(xmin);
  emit src->newXMax(xmax);
  emit src->newYMin(ymin);
  emit src->newYMax(ymax);
}

void Plotter_Implementation::resizeAxes(double dx, double dy) {
  xmin = old_vals.xmin+dx/2;
  xmax = old_vals.xmax-dx/2;
  ymin = old_vals.ymin+dy/2;
  ymax = old_vals.ymax-dy/2;

  emit src->newXMin(xmin);
  emit src->newXMax(xmax);
  emit src->newYMin(ymin);
  emit src->newYMax(ymax);
}

void Plotter_Implementation::zoomToCenter(int wx, int wy, double f) {
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

  emit src->newXMin(xmin);
  emit src->newXMax(xmax);
  emit src->newYMin(ymin);
  emit src->newYMax(ymax);
  setCenter((xmax+xmin)/2, (ymax+ymin)/2);
}

void Plotter::wheelEvent(QWheelEvent *e) {
  impl->zoomToCenter(e->x(), e->y(), e->delta() < 0 ? Plotter_Implementation::zoomFactor : 1.0/Plotter_Implementation::zoomFactor);
}

void Plotter::mousePressEvent(QMouseEvent *e) {
  impl->leftPressed = false;
  impl->key_mod = e->modifiers();

  if(e->button() == Qt::LeftButton) {
    impl->leftPressed = true;
    setCursor(Qt::ClosedHandCursor);
    impl->old_vals.anchorMX = (impl->xmax+impl->xmin)/2;
    impl->old_vals.anchorMY = (impl->ymax+impl->ymin)/2;
    impl->old_vals.anchorWX = e->x();
    impl->old_vals.anchorWY = e->y();

    if(impl->key_mod == Qt::ShiftModifier) {
      setCursor(Qt::SizeAllCursor);
      impl->old_vals.xmin = impl->xmin;
      impl->old_vals.xmax = impl->xmax;
      impl->old_vals.ymin = impl->ymin;
      impl->old_vals.ymax = impl->ymax;
    }
  }
}

void Plotter::mouseReleaseEvent(QMouseEvent*) {
  setCursor(Qt::OpenHandCursor);
}

void Plotter::mouseMoveEvent(QMouseEvent *e) {
  int dx = e->x() - impl->old_vals.anchorWX, dy = impl->old_vals.anchorWY - e->y();


  if(impl->leftPressed)
    switch(impl->key_mod) {
    case Qt::NoModifier :
      impl->setCenter(impl->old_vals.anchorMX - dx/impl->screen_params.xstep,
		      impl->old_vals.anchorMY - dy/impl->screen_params.ystep); break;
    case Qt::ShiftModifier :
      impl->resizeAxes(dx/impl->screen_params.xstep,
		       dy/impl->screen_params.ystep);break;
    default: break;
    }
}

void Plotter::setVerticalCorrection(bool v) {
  impl->verticalCorrection = v;
}
