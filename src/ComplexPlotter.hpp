#ifndef _COMPLEX_PLOTTER_HPP_
#define _COMPLEX_PLOTTER_HPP_

#include <complex>

#include "Plotter.hpp"
#include "MathFunction.hpp"

class QImage;
class QLabel;

class ComplexPlotter : public Plotter {
public:
  ComplexPlotter(QWidget *parent=0);
  ~ComplexPlotter();

  void setRayThickness(double rt) { mRayThickness = rt; }
  void setRayOpacity(double ro) { mRayOpacity = ro; }
  void setCircleThickness(double ct) { mCircleThickness = ct; }
  void setCircleOpacity(double co) { mCircleOpacity = co; }
  void setInfinityThreshold(int it) { mInfinityThreshold = it; }

  MathFunction<std::complex<double> >& getFunction() const { return *mF; }

  void doARepaint();

  // overwrites abstract declaration from Exportable
  void paintIt(QPaintDevice*, QPainter::RenderHints=0) const;

protected:
  void paintEvent(QPaintEvent *);

private:
  double mRayThickness, mRayOpacity, mCircleThickness, mCircleOpacity;
  int mInfinityThreshold;

  QImage *mImage;
  QLabel *mLabel;
  qreal mUnitCircleColor[3];
  MathFunction<std::complex<double> > *mF;
  bool mRepaintEnabled;

  QRgb getBlendedColor(const QColor &col, double l, double lambda, double mu, double nu) const;
};
#endif
