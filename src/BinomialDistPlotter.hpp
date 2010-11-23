#ifndef _BINOMIAL_DIST_PLOTTER_HPP_
#define _BINOMIAL_DIST_PLOTTER_HPP_

#include "Plotter.hpp"

// classes defined elsewhere
class MathFunction;
class QPainter;
class QPaintDevice;

class BinomialDistPlotter : public Plotter {
public:
  BinomialDistPlotter(QWidget *parent=0);

  void setN(int n) { mN = n; }
  void setP(double p) { mP = p; }
  void setKMin(int kmin) { mKmin = kmin; }
  void setKMax(int kmax) { mKmax = kmax; }
  void setPlotPdf(bool b) { mPdf = b; }
  void paintIt(QPaintDevice*, QPainter::RenderHints=0) const;

private:
  int mN;
  double mP;
  int mKmin, mKmax;

  // plot probability distribution function
  bool mPdf;
};
#endif
