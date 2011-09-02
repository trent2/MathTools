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

/*  file: --- MathTools.cpp --- */

#include <QtGui/QMainWindow>

#include "HypTestTab.hpp"
#include "BinomialDistTab.hpp"
#include "PlotTab.hpp"
#include "ComplexTab.hpp"
#include "MathTools.hpp"
#include "About.hpp"

MathTools::MathTools(QWidget *parent, Qt::WFlags f) : QMainWindow(parent, f)
{
  setupUi(this);
}

void MathTools::on_plotAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new PlotTab(mainTabWidget), "Plot"));
}

void MathTools::on_complexPlotAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new ComplexTab(mainTabWidget), "Complex Plot"));
}

void MathTools::on_binomialDistAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new BinomialDistTab(mainTabWidget), "Binomial Distribution"));
}

void MathTools::on_testingAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new HypTestTab(mainTabWidget), "Testing"));
}

void MathTools::on_aboutAction_triggered() {
  AboutDialog(this).exec();
}
