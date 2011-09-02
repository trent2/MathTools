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

/*  file: --- ComplexPlotter.hpp --- */

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
