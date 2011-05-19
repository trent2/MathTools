#ifndef _HYP_TEST_PLOTTER_HPP_
#define _HYP_TEST_PLOTTER_HPP_

#include "BinomialDistPlotter.hpp"

class HypTestPlotter : public BinomialDistPlotter {

Q_OBJECT

public:
  enum TestType { left=1, right=2, both=4 };

  HypTestPlotter(QWidget *parent=0, TestType tt=left);

  void setKLow(int kl) { klow = kl; }
  void setKHigh(int kh) { khigh = kh; }
  void setTestResult(int tr) { testresult = tr; }
  void setTest(TestType tt) { testtype = tt; }

  virtual void paintIt(QPaintDevice*, QPainter::RenderHints=0) const;

private:
  int klow, khigh;
  TestType testtype;
  int testresult;

  void blotBar(int, QPainter&, const PlotHelp::cs_params&) const;

  static const QString regOfRejectionText;
};
#endif
