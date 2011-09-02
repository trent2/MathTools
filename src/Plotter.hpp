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

/*  file: --- Plotter.hpp --- */

#ifndef _PLOTTER_HPP_
#define _PLOTTER_HPP_

#include <QtGui/QFrame>
#include <QtGui/QPainter>

#include "PlotterHelper.hpp"
#include "Exportable.hpp"

// classes defined elsewhere
class QPaintEvent;
class QMouseEvent;
class QWheelEvent;
class QPaintDevice;

// Pimpl idiom
class Plotter_Implementation;

class Plotter : public QFrame, public Exportable {

Q_OBJECT

friend class Plotter_Implementation;

public:
  enum PlotterOptions {Standard=0, AutoTicksX=1, AutoTicksY=2, DrawGridX=4, DrawGridY=8,
		       ZoomFixHorizontal=16, ZoomFixVertical=32};

  Plotter(QWidget *parent=0, PlotterOptions opt = Plotter::Standard);
  virtual ~Plotter();

  void setXMin(double xm);
  void setXMax(double xm);
  void setYMin(double ym);
  void setYMax(double ym);
  double xMin() const;
  double xMax() const;
  double yMin() const;
  double yMax() const;
  void setXTicks(double xt);
  void setYTicks(double yt);
  void setCompAutoXTicks(bool b);
  void setCompAutoYTicks(bool b);
  void setDrawGridX(bool b);
  void setDrawGridY(bool b);
  void setOptions(int opt);

protected:  // all these methods are overwritten
  void mouseMoveEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *);
  Plotter_Implementation *impl;

  /*** Determine parameters of paint device such as height, width,
   *   number of given device units (in case of screen, this is pixel)
   *   per coordinate system unit, distance between axis labels in device units
   **/
  PlotHelp::cs_params computeCSParameters(QPaintDevice *) const;

  /*** Plot the coordinate grid on a QPainter p.
   **/
  void plotGrid(QPainter &p) const;
  void paintEvent(QPaintEvent *);

private:

Q_SIGNALS:
  void newXMin(double);
  void newXMax(double);
  void newYMin(double);
  void newYMax(double);

  void newXTicks(double);
  void newYTicks(double);
};
#endif
