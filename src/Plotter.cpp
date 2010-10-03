#include <vector>
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
				    compAutoXTicks(false), compAutoYTicks(false),
				    leftPressed(false) {
  // test code: generate a test function
  mfunc.push_back(MathFunction());
}

Plotter::~Plotter() {
  mfunc.clear();
}

void Plotter::paintEvent(QPaintEvent *) {
  computeCSParameters();

  QPainterPath pathstroke;
  QPainter p(this);

  //  p.setRenderHint(QPainter::Antialiasing);

  p.setPen(QPen(Qt::black, 1));
  plotGrid(p);

  p.setPen(QPen(Qt::red, 2));

  MathFunction *fct = & mfunc[0];

  pathstroke.moveTo(0, winheight-(fct->eval(-xmin)-ymin)*ystep);
  for(int x=1; x<=winwidth; ++x)
    pathstroke.lineTo(x, winheight-roundi((fct->eval(x/xstep+xmin)-ymin)*ystep));

  p.drawPath(pathstroke);
}

void Plotter::plotGrid(QPainter &p) {
  double tick;
  
  // is there a y-axis?
  if(sign(xmin)*sign(xmax) <= 0) {
    tick = std::floor(ymin/yticks)*yticks;

    // draw tick marks
    while(tick <= ymax) {
      p.drawLine(roundi(-xmin*xstep)-3, winheight-roundi((tick-ymin)*ystep), roundi(-xmin*xstep)+3, winheight-roundi((tick-ymin)*ystep));
      tick += yticks;
    }

    // draw axis
    p.drawLine(roundi(-xmin*xstep), 0, roundi(-xmin*xstep), winheight);
  }

  // is there an x-axis?
  if(sign(ymin)*sign(ymax) <= 0) {
    tick = std::floor(xmin/xticks)*xticks;

    // draw tick marks
   while(tick <= xmax) {
      p.drawLine(roundi((tick-xmin)*xstep), winheight-roundi(-ymin*ystep)+3, roundi((tick-xmin)*xstep), winheight-roundi(-ymin*ystep)-3);
      tick += xticks;
    }
    p.drawLine(0, winheight-roundi(-ymin*ystep), winwidth, winheight-roundi(-ymin*ystep));
  }
}

void Plotter::computeCSParameters() {
  winwidth = width()-1;
  winheight = height()-1;

  xstep = winwidth/(xmax-xmin);
  ystep = winheight/(ymax-ymin);
  if(compAutoXTicks) {
    xticks = (xmax-xmin)*((double)tickPixelDistForAuto)/winwidth;
    xticks = round(xticks*2)/2;
    emit newXTicks(xticks);
  }
  if(compAutoYTicks) {
    yticks = (ymax-ymin)*((double)tickPixelDistForAuto)/winheight;
    yticks = round(yticks*2)/2;
    emit newYTicks(yticks);
  }
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

  tw = winwidth/(xmax-xmin);
  th = winheight/(ymax-ymin);

  // shift (x,y) back
  xmin -= dw/tw;
  xmax -= dw/tw;
  ymin -= dh/th;
  ymax -= dh/th;

  emit newXMin(xmin);
  emit newXMax(xmax);
  emit newYMin(ymin);
  emit newYMax(ymax);
}

void Plotter::wheelEvent(QWheelEvent *e) {
  zoomToCenter(e->x(), e->y(), e->delta() < 0 ? zoomFactor : 1.0/zoomFactor);
}

void Plotter::mousePressEvent(QMouseEvent *e) {
  leftPressed = false;
  if(e->button() == Qt::LeftButton) {
    leftPressed = true;
    anchorMX = (xmax+xmin)/2;
    anchorMY = (ymax+ymin)/2;
    anchorWX = e->x();
    anchorWY = e->y();
    //    setCenter(e->x()/xstep + xmin, (winheight-e->y())/ystep + ymin);
  }
}

void Plotter::mouseMoveEvent(QMouseEvent *e) {
  int dx = e->x() - anchorWX, dy = anchorWY - e->y();

  if(leftPressed)
    setCenter(anchorMX - dx/xstep, anchorMY - dy/ystep);
}

/** Slots **/

void Plotter::setXMin(double xmin) {
  this->xmin = xmin;
  update();
}

void Plotter::setXMax(double xmax) {
  this->xmax = xmax;
  update();
}

void Plotter::setYMin(double ymin) {
  this->ymin = ymin;
  update();
}

void Plotter::setYMax(double ymax) {
  this->ymax = ymax;
  update();
}

void Plotter::setXTicks(double xticks) {
  this->xticks = xticks;
  update();
}

void Plotter::setYTicks(double yticks) {
  this->yticks = yticks;
  update();
}

void Plotter::autoXTicks(bool b) {
  compAutoXTicks = b;
  update();
}

void Plotter::autoYTicks(bool b) {
  compAutoYTicks = b;
  update();
}
