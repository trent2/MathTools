#include <vector>
#include <cmath>
#include <fstream>

#include <QtGui/QFrame>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

#include "Plotter.hpp"
#include "MathFunction.hpp"
#include "Calc.hpp"

Plotter::Plotter(QWidget *parent) : QFrame(parent),
				    xmin(-10), xmax(10), ymin(-10), ymax(10), xticks(1), yticks(1),
				    compAutoXTicks(false), compAutoYTicks(false), leftPressed(false),
				    verticalCorrection(true) {
  // test code: generate a test function
  
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
	
	if(verticalCorrection && std::abs(y-yold)>2*(ymax-ymin) ||
	   std::isnan(y) || std::isinf(y))
	  nextIsMove = true;
	else
	  if(nextIsMove) {
	    pathstroke.moveTo(x, winheight-roundi((y-ymin)*ystep));
	    nextIsMove = false;
	  } else {
	    pathstroke.lineTo(x, winheight-roundi((y-ymin)*ystep));
	  }
	yold = y;
      }
      p.drawPath(pathstroke);
    }
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

    tick = std::floor(ymin/descDistY)*descDistY;
    // draw numbers
    while(tick <= ymax) {
      p.drawText(roundi(-xmin*xstep)-15, winheight-roundi((tick-ymin)*ystep)+5, QString::number(tick, 'g', 2));
      tick += descDistY;
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

    tick = std::floor(xmin/descDistX)*descDistX;
    // draw numbers
    while(tick <= xmax) {
      p.drawText(roundi((tick-xmin)*xstep)-7, winheight-roundi(-ymin*ystep)+20, QString::number(tick, 'g', 2));
      tick += descDistX;
    }

    // draw axis
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
    old_vals.anchorMX = (xmax+xmin)/2;
    old_vals.anchorMY = (ymax+ymin)/2;
    old_vals.anchorWX = e->x();
    old_vals.anchorWY = e->y();

    if(key_mod == Qt::ShiftModifier) {
      old_vals.xmin = xmin;
      old_vals.xmax = xmax;
      old_vals.ymin = ymin;
      old_vals.ymax = ymax;
    }
  }
}

void Plotter::mouseMoveEvent(QMouseEvent *e) {
  int dx = e->x() - old_vals.anchorWX, dy = old_vals.anchorWY - e->y();

  if(leftPressed)
    switch(key_mod) {
    case Qt::NoModifier    : setCenter(old_vals.anchorMX - dx/xstep, old_vals.anchorMY - dy/ystep); break;
    case Qt::ShiftModifier : resizeAxes(dx/xstep, dy/ystep);break;
    default: break;
    }
}

/** Slots **/

void Plotter::setXMin(double xmin) {
  this->xmin = xmin;
  if(xmax<xmin) {
    xmax = xmin+.5;
    emit newXMax(xmax);
  }
  emit newXMin(xmin);
  update();
}

void Plotter::setXMax(double xmax) {
  this->xmax = xmax;
  if(xmax<xmin) {
    xmin = xmax-.5;
    emit newXMin(xmin);
  }
  emit newXMax(xmax);
  update();
}

void Plotter::setYMin(double ymin) {
  this->ymin = ymin;
  if(ymax<ymin) {
    ymax = ymin+.5;
    emit newYMax(ymax);
  }
  emit newYMin(ymin);
  update();
}

void Plotter::setYMax(double ymax) {
  this->ymax = ymax;
  if(ymax<ymin) {
    ymin = ymax-.5;
    emit newYMin(ymin);
  }
  emit newYMax(ymax);
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

void Plotter::setStandardWindow() {
  xmin = -10;
  xmax = 10;
  ymin = -10;
  ymax = 10;
  emit newXMin(xmin);
  emit newXMax(xmax);
  emit newYMin(ymin);
  emit newYMax(ymax);
}

void Plotter::f1ToFile() {
  std::ofstream debug_file;
  debug_file.open("f1_vals.txt");

  for(int x=0; x<=winwidth; ++x) {
    debug_file << "(" << xmin+x/xstep << ", " << mfunc[0](xmin+x/xstep) << ") ";
    if(!(x%5))
      debug_file << std::endl;
  }
  debug_file.close();
}
