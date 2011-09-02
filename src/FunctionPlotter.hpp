#ifndef _FUNCTION_PLOTTER_HPP_
#define _FUNCTION_PLOTTER_HPP_

#include "Plotter.hpp"
#include "MathFunction.hpp"

// classes defined elsewhere
inline template class MathFunction<double>;
class QPainter;
class QPaintDevice;

class FunctionPlotter_Implementation;

class FunctionPlotter : public Plotter {

Q_OBJECT

friend class FunctionPlotter_Implementation;

public:
  FunctionPlotter(QWidget *parent=0);
  ~FunctionPlotter();

  
  MathFunction<double>& getFunction(int index);
  void paintIt(QPaintDevice*, QPainter::RenderHints=0) const;

public slots:
  void setVerticalCorrection(bool v);

private:
  FunctionPlotter_Implementation *impl_fp;
};
#endif
