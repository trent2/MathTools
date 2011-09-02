/*  MathTools 
    Copyright (C) 2010, 2011, 2012 Robert Spillner <Robert.Spillner@gmx.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*  file: --- HypTestPlotter.hpp --- */

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
