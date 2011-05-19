#include <algorithm>

#include <QtGui/QFrame>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

#include "Plotter.hpp"
#include "Calc.hpp"

class Plotter_Implementation {
private:
  Plotter_Implementation(Plotter *pSrc, Plotter::PlotterOptions opt);

  friend class Plotter;
  Plotter *src;


  static const int tickPixelDistForAuto;
  static const int descAutoPixelDist;
  static const double zoomFactor;

  // minimum and maximum of real view window
  double xmin, xmax, ymin, ymax;
  // tick-lenght for axis ticks
  double xticks, yticks;

  // for mouse movement of the graph
  bool leftPressed;

  // options bit-vector
  int options_bv;
  // modifiers hit when mouse was pressed
  unsigned int key_mod;

  // old values for tracking resize and move operations
  PlotHelp::old_vals_t old_vals;
  PlotHelp::cs_params screen_params;

  void plotGrid(QPainter&, qreal) const;
  void computeAutoTicks();
  PlotHelp::cs_params computeCSParameters(const QPaintDevice*) const;
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
const double Plotter_Implementation::zoomFactor = 1.1;

Plotter_Implementation::Plotter_Implementation(Plotter *pSrc, Plotter::PlotterOptions opt) :
  src(pSrc), xticks(1), yticks(1), leftPressed(false), options_bv(Plotter::Standard) {
  src->setCursor(Qt::OpenHandCursor);
}

Plotter::Plotter(QWidget *parent, PlotterOptions opt) :
  QFrame(parent), impl(new Plotter_Implementation(this, opt)) { }

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

double Plotter::xMin() const {
  return impl->xmin;
}

double Plotter::xMax() const {
  return impl->xmax;
}

double Plotter::yMin() const {
  return impl->ymin;
}

double Plotter::yMax() const {
  return impl->ymax;
}

void Plotter::setCompAutoXTicks(bool b) {
  int *o = &impl->options_bv;
  *o =  (b ? *o | Plotter::AutoTicksX : *o & ~Plotter::AutoTicksX);
}

void Plotter::setCompAutoYTicks(bool b) {
  int *o = &impl->options_bv;
  *o =  (b ? *o | Plotter::AutoTicksY : *o & ~Plotter::AutoTicksY);
}

void Plotter::setDrawGridX(bool b) {
  int *o = &impl->options_bv;
  *o =  (b ? *o | Plotter::DrawGridX : *o & ~Plotter::DrawGridX);
}

void Plotter::setDrawGridY(bool b) {
  int *o = &impl->options_bv;
  *o =  (b ? *o | Plotter::DrawGridY : *o & ~Plotter::DrawGridY);
}

void Plotter::setOptions(int opt) {
  impl->options_bv = opt;
}


void Plotter::paintEvent(QPaintEvent *e) {
  // do non-const operations
  impl->computeAutoTicks();
  impl->screen_params = impl->computeCSParameters(this);
  paintIt(this);
}

void Plotter_Implementation::plotGrid(QPainter &p, qreal res_factor) const {
  double tick;
  QSize textSizeMax;
  QFont myFont;
  // QPen myPen;
  QString number;
  std::vector<QString> axisLabels;
  const PlotHelp::cs_params param = computeCSParameters(p.device());
  bool drawGridX = options_bv & Plotter::DrawGridX,
    drawGridY = options_bv & Plotter::DrawGridY;

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
      p.setPen(QPen(Qt::black, res_factor, Qt::DotLine));
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
    p.setPen(QPen(Qt::black, res_factor));

    // draw axis
    if(xbot>=0 && xtop<=param.winwidth)
       p.drawLine(calc::roundi(-xmin*param.xstep), 0, calc::roundi(-xmin*param.xstep), param.winheight);
  }

  // is there an x-axis?
  if((ybot>=0 && ytop <=param.winheight) || drawGridX) {
    tick = std::ceil(xmin/xticks)*xticks;

    if(drawGridX)
      p.setPen(QPen(Qt::black, res_factor, Qt::DotLine));
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

    p.setPen(QPen(Qt::black, res_factor));
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
PlotHelp::cs_params Plotter_Implementation::computeCSParameters(const QPaintDevice *paintDev) const {
  PlotHelp::cs_params ret;
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
  if(options_bv & Plotter::AutoTicksX) {
    xticks = normalizeTickValue((xmax-xmin)*((double)tickPixelDistForAuto)/src->width());
    emit src->newXTicks(xticks);
  }
  if(options_bv & Plotter::AutoTicksY) {
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
  
  if(!(options_bv & Plotter::ZoomFixHorizontal)) {
    xmin += (x-mx);
    xmax += (x-mx);
    emit src->newXMin(xmin);
    emit src->newXMax(xmax);
  }
  if(!(options_bv & Plotter::ZoomFixVertical)) {
    ymin += (y-my);
    ymax += (y-my);
    emit src->newYMin(ymin);
    emit src->newYMax(ymax);
  }
}

void Plotter_Implementation::resizeAxes(double dx, double dy) {
  if(!(options_bv & Plotter::ZoomFixHorizontal)) {
    xmin = old_vals.xmin+dx/2;
    xmax = old_vals.xmax-dx/2;
    emit src->newXMin(xmin);
    emit src->newXMax(xmax);
  }

  if(!(options_bv & Plotter::ZoomFixVertical)) {
    ymin = old_vals.ymin+dy/2;
    ymax = old_vals.ymax-dy/2;

    emit src->newYMin(ymin);
    emit src->newYMax(ymax);
  }
}

void Plotter_Implementation::zoomToCenter(int wx, int wy, double f) {
  int dw = wx-screen_params.winwidth/2;
  double tw = screen_params.xstep;
  int dh = screen_params.winheight/2-wy;
  double th = screen_params.ystep;

  // shift (x,y) to center
  if(!(options_bv & Plotter::ZoomFixHorizontal)) {
    xmin += dw/tw;
    xmax += dw/tw;

    tw = (xmax-xmin)*((f-1)/(2*f));

    xmin += tw;
    xmax -= tw;

    tw = screen_params.winwidth/(xmax-xmin);

    xmin -= dw/tw;
    xmax -= dw/tw;

    // shift (x,y) back

    emit src->newXMin(xmin);
    emit src->newXMax(xmax);
  }

  if(!(options_bv & Plotter::ZoomFixVertical)) {
    ymin += dh/th;
    ymax += dh/th;  

    th = (ymax-ymin)*((f-1)/(2*f));

    // zoom
    ymin += th;
    ymax -= th;

    th = screen_params.winheight/(ymax-ymin);

    ymin -= dh/th;
    ymax -= dh/th;

    emit src->newYMin(ymin);
    emit src->newYMax(ymax);
  }
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

PlotHelp::cs_params Plotter::computeCSParameters(QPaintDevice *d) const {
  return impl->computeCSParameters(d);
}

void Plotter::plotGrid(QPainter &p) const {
  impl->plotGrid(p, PlotHelp::getResFactor(p.device(), this));
}
