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

/*  file: --- BinomialDistPlotter.hpp --- */

#ifndef _BINOMIAL_DIST_PLOTTER_HPP_
#define _BINOMIAL_DIST_PLOTTER_HPP_

#include "Plotter.hpp"

class BinomialDistPlotter : public Plotter {
public:
  BinomialDistPlotter(QWidget *parent=0);

  void setN(int n) { mN = n; }
  void setP(double p) { mP = p; }
  void setKMin(int kmin) { mKmin = kmin; }
  void setKMax(int kmax) { mKmax = kmax; }
  void setPlotPdf(bool b) { mPdf = b; }
  virtual void paintIt(QPaintDevice*, QPainter::RenderHints=0) const;


protected:
  int mN;
  double mP;
  int mKmin, mKmax;

  // plot probability distribution function
  bool mPdf;

  void paintIt(QPainter &p, const QPainter::RenderHints&) const;
};
#endif
