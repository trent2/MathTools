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

/*  file: --- BinomialDistTab.hpp --- */

#ifndef _BIN_DIST_TAB_HPP_
#define _BIN_DIST_TAB_HPP_
#include "ui_binomialdist.h"

class BinomialDistTab : public QWidget, private Ui::BinomialDistForm {

Q_OBJECT

public:
  BinomialDistTab(QWidget* parent=0);

private slots:  // slots are auto-connected
  void on_nSpinBox_valueChanged(int);
  void on_pDSpinBox_valueChanged(double);

  void on_xmaxSpinBox_valueChanged(int);
  void on_yminDSpinBox_valueChanged(double);
  void on_ymaxDSpinBox_valueChanged(double);

  void on_binPlotter_newXMax(double);
  void on_binPlotter_newYMin(double);
  void on_binPlotter_newYMax(double);

  void on_distRButton_toggled(bool);

  void on_joinCheckBox_toggled(bool);

  void on_kminSpinBox_valueChanged(int);
  void on_kmaxSpinBox_valueChanged(int);

  void updateProbability();

  // export
  void on_exportPushButton_clicked();

};
#endif
