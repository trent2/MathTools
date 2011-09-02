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

/*  file: --- ComplexTab.hpp --- */

#ifndef _COMPLEX_TAB_HPP_
#define _COMPLEX_TAB_HPP_
#include "ui_complextab.h"

class ComplexTab : public QWidget, private Ui::ComplexPlotForm {

Q_OBJECT

public:
  ComplexTab(QWidget* parent=0);

private slots:  // slots are auto-connected
  void on_repaintPushButton_clicked();
  void on_standardPushButton_clicked();

  void on_plotter_newXMin(double xmin);
  void on_plotter_newXMax(double xmax);
  void on_plotter_newYMin(double ymin);
  void on_plotter_newYMax(double ymax);

  void on_xminDSpinBox_valueChanged(double);
  void on_xmaxDSpinBox_valueChanged(double);
  void on_yminDSpinBox_valueChanged(double);
  void on_ymaxDSpinBox_valueChanged(double);

  void on_rayThicknessDSpinBox_valueChanged(double);
  void on_rayOpacityDSpinBox_valueChanged(double);
  void on_circleThicknessDSpinBox_valueChanged(double);
  void on_circleOpacityDSpinBox_valueChanged(double);
  void on_infThresholdSpinBox_valueChanged(int);
};
#endif
