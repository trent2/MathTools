#ifndef _FUNCTION_PLOTTER_HPP_
#define _FUNCTION_PLOTTER_HPP_

#include "Plotter.hpp"

// classes defined elsewhere
class MathFunction;
class QPainter;
class QPaintDevice;

class FunctionPlotter_Implementation;

class FunctionPlotter : public Plotter {

Q_OBJECT

friend class FunctionPlotter_Implementation;

public:
  FunctionPlotter(QWidget *parent=0);
  ~FunctionPlotter();

  MathFunction& getFunction(int index);
  void paintIt(QPaintDevice*, QPainter::RenderHints=0) const;

public slots:
  void setVerticalCorrection(bool v);

private:
  FunctionPlotter_Implementation *impl_fp;

Q_SIGNALS:
  void newXMin(double);
  void newXMax(double);
  void newYMin(double);
  void newYMax(double);

  void newXTicks(double);
  void newYTicks(double);
};
#endif
