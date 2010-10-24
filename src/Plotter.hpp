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

  // minimum and maximum of real view window
  double xmin, xmax, ymin, ymax;
  // tick-lenght for axis ticks
  double xticks, yticks;
  // compute x- and y-ticks automatically
  bool compAutoXTicks, compAutoYTicks;
  // draw grid
  bool drawGrid;
protected:  // all these methods are overwritten
  void paintEvent(QPaintEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *);

private:
  int winwidth, winheight;

  static const int tickPixelDistForAuto = 20;
  static const int descAutoPixelDist = 100;
  static const double zoomFactor = 1.2;

  // flag to determine whether to draw vertical lines
  bool verticalCorrection;

  // for mouse movement of the graph
  bool leftPressed;
  // modifiers hit when mouse was pressed
  unsigned int key_mod;

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
  double normalizeTickValue(double) const;

  // set (x,y) as the center of the cs
  void setCenter(double x, double y);

  // change axes size by dx, dy
  void resizeAxes(double dx, double dy);

  // f>1 indicates to zoom in, f<1 zooms out
  void zoomToCenter(int, int, double f);

public slots:
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
