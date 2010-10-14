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

  void update();
  MathFunction& getFunction(int index);

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
  static const int descAutoPixelDist = 100;
  static const double zoomFactor = 1.2;

  // compute x- and y-ticks automatically
  bool compAutoXTicks, compAutoYTicks;

  // draw grid
  bool drawGrid;

  // flag to determine whether to draw vertical lines
  bool verticalCorrection;

  // for mouse movement of the graph
  bool leftPressed;
  // modifiers hit when mouse was pressed
  unsigned int key_mod;

  // tick-lenght for axis ticks
  double xticks, yticks;
  // distance between number label on axes
  double descDistX, descDistY;

  // pixels per unit
  double xstep, ystep;

  // old values for tracking resize and move operations
  struct old_vals_t {
    double xmin, xmax, ymin, ymax;
    // mean(xmax,xmin), mean(ymax,ymin)
    double anchorMX, anchorMY;
    // position of mouse move start (in pixel coords)
    int anchorWX, anchorWY;
  } old_vals;


  std::vector<MathFunction> mfunc;

  void plotGrid(QPainter&);
  void computeCSParameters();

  // set (x,y) as the center of the cs
  void setCenter(double x, double y);

  // change axes size by dx, dy
  void resizeAxes(double dx, double dy);

  // f>1 indicates to zoom in, f<1 zooms out
  void zoomToCenter(int, int, double f);

public slots:
  void setXMin(double xmin);
  void setXMax(double xmax);
  void setYMin(double ymin);
  void setYMax(double ymax);

  void setXTicks(double xticks);
  void setYTicks(double yticks);
  void autoXTicks(bool);
  void autoYTicks(bool);
  void toggleGrid(bool);

  void setStandardWindow();

  void f1ToFile();
  void setVerticalCorrection(bool v) { verticalCorrection = v; }

signals:
  void newXMin(double);
  void newXMax(double);
  void newYMin(double);
  void newYMax(double);

  void newXTicks(double);
  void newYTicks(double);
};
#endif
