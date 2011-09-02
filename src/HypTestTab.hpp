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

/*  file: --- HypTestTab.hpp --- */

#ifndef _HYP_TEST_HPP_
#define _HYP_TEST_HPP_
#include "ui_hyptest.h"

class HypTestTab : public QWidget, private Ui::HypTestForm {

Q_OBJECT

public:
  HypTestTab(QWidget* parent=0);
private:
  int k1, k2;
private slots:  // slots are auto-connected
  void on_nSpinBox_valueChanged(int);
  void on_xSpinBox_valueChanged(int);
  void on_pDSpinBox_valueChanged(double);
  void on_alphaDSpinBox_valueChanged(double);

  void on_xmaxSpinBox_valueChanged(int);
  void on_yminDSpinBox_valueChanged(double);
  void on_ymaxDSpinBox_valueChanged(double);

  void on_hypPlotter_newXMax(double);
  void on_hypPlotter_newYMin(double);
  void on_hypPlotter_newYMax(double);

  void on_leftRButton_toggled(bool);
  void on_rightRButton_toggled(bool);
  void on_bothRButton_toggled(bool);

  void updateV();

  // export
  void on_exportPushButton_clicked();
};
#endif
