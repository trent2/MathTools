#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>

#include <QtGui/QFrame>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

#include "Plotter.hpp"
#include "MathFunction.hpp"
#include "Calc.hpp"

Plotter::Plotter(QWidget *parent) : QFrame(parent),
				    xmin(-10), xmax(10), ymin(-10), ymax(10), xticks(1), yticks(1),
				    compAutoXTicks(false), compAutoYTicks(false), drawGrid(false),
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
  bool nextIsMove;
  double y, yold;

  computeCSParameters();
  const double xustep = 1.0/xstep;

  QPainter p(this);

  //  p.setRenderHint(QPainter::Antialiasing);

  p.setPen(QPen(Qt::black, 1));
  plotGrid(p);

  // draw functions
  for(int i=0; i<4; ++i)
    if(mfunc[i].parse()) {  // function term is parsable hence evaluable
      mfunc[i].setUStepsize(xustep);

      QPainterPath pathstroke;
      nextIsMove = true;

      p.setPen(QPen(mfunc[i].getColor(), 2));

      for(int x=0; x<=winwidth; ++x) {
	// compute value of functions
	y = mfunc[i](xmin+x/xstep);
	if(!x)
	  yold = y;
	
	if((verticalCorrection && std::abs(y-yold)>2*(ymax-ymin)) ||
	   std::isnan(y) || std::isinf(y))
	  nextIsMove = true;
	else
	  if(nextIsMove) {
	    pathstroke.moveTo(x, winheight-calc::roundi((y-ymin)*ystep));
	    nextIsMove = false;
	  } else {
	    pathstroke.lineTo(x, winheight-calc::roundi((y-ymin)*ystep));
	  }
	yold = y;
      }
      p.drawPath(pathstroke);
    }
}

void Plotter::plotGrid(QPainter &p) {
  double tick;
  const double
    xtop = (drawGrid ? winwidth : calc::roundi(-xmin*xstep)+3),
    xbot = (drawGrid ? 0 : calc::roundi(-xmin*xstep)-3),
    ytop = (drawGrid ? 0 : winheight-calc::roundi(-ymin*ystep)-3),
    ybot = (drawGrid ? winheight : winheight-calc::roundi(-ymin*ystep)+3);

  
  // is there a y-axis?
  if((xbot>=0 && xtop<=winwidth) || drawGrid) {
    tick = std::floor(ymin/yticks)*yticks;

    if(drawGrid)
      p.setPen(Qt::DashLine);
    // draw tick marks
    while(tick <= ymax) {
      p.drawLine(std::max(0.0,xbot), winheight-calc::roundi((tick-ymin)*ystep),
		 std::min((double)winwidth, xtop), winheight-calc::roundi((tick-ymin)*ystep));
      tick += yticks;
    }

    tick = std::floor(ymin/descDistY)*descDistY;
    // draw numbers
    while(tick <= ymax) {
      if(std::abs(tick) < 3*std::numeric_limits<double>::epsilon())
	tick = 0;
      if(tick)
	p.drawText(calc::roundi(-xmin*xstep)-15, winheight-calc::roundi((tick-ymin)*ystep)+5, QString::number(tick, 'g', 2));
      tick += descDistY;
    }
    p.setPen(Qt::SolidLine);

    // draw axis
    if(xbot>=0 && xtop<=winwidth)
       p.drawLine(calc::roundi(-xmin*xstep), 0, calc::roundi(-xmin*xstep), winheight);
  }

  // is there an x-axis?
  if((ybot>=0 && ytop <=winheight) || drawGrid) {
    tick = std::floor(xmin/xticks)*xticks;

    if(drawGrid)
      p.setPen(Qt::DashLine);
    // draw tick marks
    while(tick <= xmax) {
      p.drawLine(calc::roundi((tick-xmin)*xstep), std::min((double)winheight,ybot),
		 calc::roundi((tick-xmin)*xstep), std::max(0.0,ytop));
      tick += xticks;
    }

    tick = std::floor(xmin/descDistX)*descDistX;
    // draw numbers
    while(tick <= xmax) {
      if(std::abs(tick) < 3*std::numeric_limits<double>::epsilon())
	tick = 0;
      p.drawText(calc::roundi((tick-xmin)*xstep)-7, winheight-calc::roundi(-ymin*ystep)+20, QString::number(tick, 'g', 2));
      tick += descDistX;
    }

    p.setPen(Qt::SolidLine);
    // draw axis

    if(ybot>=0 && ytop <=winheight)
      p.drawLine(0, winheight-calc::roundi(-ymin*ystep), winwidth, winheight-calc::roundi(-ymin*ystep));
  }
}

void Plotter::computeCSParameters() {
  winwidth = width()-1;
  winheight = height()-1;

  xstep = winwidth/(xmax-xmin);
  ystep = winheight/(ymax-ymin);
  if(compAutoXTicks) {
    xticks = (xmax-xmin)*((double)tickPixelDistForAuto)/winwidth;
    xticks = normalizeTickValue(xticks);
    emit newXTicks(xticks);
  }
  if(compAutoYTicks) {
    yticks = (ymax-ymin)*((double)tickPixelDistForAuto)/winheight;
    yticks = normalizeTickValue(yticks);
    emit newYTicks(yticks);
  }

  // compute distances between numbers labels on axes
  descDistX = (xmax-xmin)*((double)descAutoPixelDist)/winwidth;
  if(xticks>descDistX)
    descDistX = xticks;
  else
    descDistX = std::ceil(descDistX/xticks)*xticks;

  descDistY = (ymax-ymin)*((double)descAutoPixelDist)/winheight;
  if(yticks>descDistY)
    descDistY = yticks;
  else
    descDistY = std::ceil(descDistY/yticks)*yticks;
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
  int dw = wx-winwidth/2;
  double tw = xstep;
  int dh = winheight/2-wy;
  double th = ystep;

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
  tw = winwidth/(xmax-xmin);
  th = winheight/(ymax-ymin);

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
      setCenter(old_vals.anchorMX - dx/xstep, old_vals.anchorMY - dy/ystep); break;
    case Qt::ShiftModifier :
      resizeAxes(dx/xstep, dy/ystep);break;
    default: break;
    }
}
