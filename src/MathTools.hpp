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

/*  file: --- MathTools.hpp --- */

#ifndef __MATHTOOLS_HPP_
#define __MATHTOOLS_HPP_

#include "ui_mathtoolsmain.h"

class QMainWindow;

class MathTools : public QMainWindow, private Ui::MathToolsMainWindow
{

Q_OBJECT

public:
  MathTools(QWidget * parent = 0, Qt::WFlags f = Qt::Window);

private slots:  // slots are auto-connected
  void on_plotAction_triggered();
  void on_complexPlotAction_triggered();
  void on_binomialDistAction_triggered();
  void on_testingAction_triggered();
  void on_aboutAction_triggered();
};

#endif // __MATHTOOLS_HPP_
