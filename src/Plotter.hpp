#ifndef _PLOTTER_HPP_
#define _PLOTTER_HPP_

#include <QtGui/QFrame>
#include <QtGui/QPainter>

#include "Exportable.hpp"

// classes defined elsewhere
class MathFunction;
class QPaintEvent;
class QMouseEvent;
class QWheelEvent;
class QFrame;
class QPaintDevice;

// Pimpl idiom
class Plotter_Implementation;

class Plotter : public QFrame, public Exportable {

Q_OBJECT

friend class Plotter_Implementation;

public:
  Plotter(QWidget *parent=0);
  ~Plotter();

  void update();
  MathFunction& getFunction(int index);
  void paintIt(QPaintDevice*, QPainter::RenderHints=0) const;

  void setXMin(double xm);
  void setXMax(double xm);
  void setYMin(double ym);
  void setYMax(double ym);
  void setXTicks(double xt);
  void setYTicks(double yt);
  void setCompAutoXTicks(bool b);
  void setCompAutoYTicks(bool b);
  void setDrawGridX(bool b);
  void setDrawGridY(bool b);

protected:  // all these methods are overwritten
  void paintEvent(QPaintEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *);

private:
  Plotter_Implementation *impl;

public slots:
  void setVerticalCorrection(bool v);

Q_SIGNALS:
  void newXMin(double);
  void newXMax(double);
  void newYMin(double);
  void newYMax(double);

  void newXTicks(double);
  void newYTicks(double);
};
#endif
