#ifndef _PLOTTER_HPP_
#define _PLOTTER_HPP_

#include <QtGui/QFrame>

// classes defined elsewhere
class MathFunction;
class QPaintEvent;
class QMouseEvent;
class QWheelEvent;
class QFrame;
class QPainter;

class Plotter : public QFrame {

Q_OBJECT

public:
  Plotter(QWidget *parent=0);
  ~Plotter();

protected:  // all these methods are overwritten
  void paintEvent(QPaintEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *);

private:

  // minimum and maximum of real view window
  double xmin, xmax, ymin, ymax;
  int winwidth, winheight;

  static const int tickPixelDistForAuto = 20;
  static const double zoomFactor = 1.2;

  // compute x- and y-ticks automatically
  bool compAutoXTicks, compAutoYTicks;

  // for mouse movement of the graph
  bool leftPressed;

  // tick-lenght
  double xticks, yticks;

  // pixels per unit
  double xstep, ystep;

  // position of mouse move start
  int anchorWX, anchorWY;
  double anchorMX, anchorMY;

  std::vector<MathFunction> mfunc;

  void plotGrid(QPainter&);
  void computeCSParameters();

  // set (x,y) as the center of the cs
  void setCenter(double x, double y);

  // factor>1 indicates to zoom in, factor<1 zooms out
  void zoomToCenter(int wx, int wy, double f);

public slots:
  void setXMin(double xmin);
  void setXMax(double xmax);
  void setYMin(double ymin);
  void setYMax(double ymax);

  void setXTicks(double xticks);
  void setYTicks(double yticks);
  void autoXTicks(bool);
  void autoYTicks(bool);

signals:
  void newXMin(double);
  void newXMax(double);
  void newYMin(double);
  void newYMax(double);

  void newXTicks(double);
  void newYTicks(double);
};
#endif
