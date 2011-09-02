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

/*  file: --- BinomialDistTab.cpp --- */

#include <algorithm>
#include <gsl/gsl_cdf.h>

#include "BinomialDistTab.hpp"
#include "ExportDialog.hpp"

BinomialDistTab::BinomialDistTab(QWidget* parent) : QWidget(parent) {
  setupUi(this);
  binPlotter->setOptions(// Plotter::ZoomFixHorizontal | 
			 Plotter::AutoTicksX | Plotter::AutoTicksY);
  updateProbability();
}

void BinomialDistTab::on_nSpinBox_valueChanged(int n) {
  binPlotter->setN(n);
  binPlotter->setXMax(n);
  binPlotter->setXMin(-1);
  binPlotter->update();
  kminSpinBox->setMaximum(n);
  kmaxSpinBox->setMaximum(n);
  updateProbability();
}

void BinomialDistTab::on_pDSpinBox_valueChanged(double p) {
  binPlotter->setP(p);
  updateProbability();
  binPlotter->update();
}

/*
void BinomialDistTab::on_binPlotter_newXMin(double xmin) {
  if(binPlotter->xMin() != -1)
    binPlotter->setXMin(-1);
}
*/

void BinomialDistTab::on_binPlotter_newXMax(double xmax) {
  if(binPlotter->xMin()<-xmax*.05)
    binPlotter->setXMin(-xmax*.05);
  xmaxSpinBox->setValue(xmax);

  binPlotter->update();
}

void BinomialDistTab::on_binPlotter_newYMin(double ymin) {
  yminDSpinBox->setValue(ymin);
  binPlotter->update();
}

void BinomialDistTab::on_binPlotter_newYMax(double ymax) {
  ymaxDSpinBox->setValue(ymax);
  binPlotter->update();
}

void BinomialDistTab::on_xmaxSpinBox_valueChanged(int xmax) {
  binPlotter->setXMax(xmax);
}

void BinomialDistTab::on_yminDSpinBox_valueChanged(double ymin) {
  double ymax = ymaxDSpinBox->value();

  if(ymax < ymin) {
    ymax = ymin+.5;
    ymaxDSpinBox->setValue(ymax);
  }
  binPlotter->setYMin(ymin);
}

void BinomialDistTab::on_ymaxDSpinBox_valueChanged(double ymax) {
  double ymin = yminDSpinBox->value();

  if(ymax < ymin) {
    ymin = ymax-.5;
    yminDSpinBox->setValue(ymin);
  }
  binPlotter->setYMax(ymax);
}

void BinomialDistTab::on_distRButton_toggled(bool b) {
  binPlotter->setPlotPdf(b);
  binPlotter->update();
}

void BinomialDistTab::on_joinCheckBox_toggled(bool b) {
  if(b)
    kmaxSpinBox->setValue(kminSpinBox->value());
}

void BinomialDistTab::on_kminSpinBox_valueChanged(int) {
  if(kminSpinBox->value() > kmaxSpinBox->value() || joinCheckBox->isChecked())
    kmaxSpinBox->setValue(kminSpinBox->value());

  binPlotter->setKMin(kminSpinBox->value());

  updateProbability();
  binPlotter->update();
}

void BinomialDistTab::on_kmaxSpinBox_valueChanged(int) {
  if(kminSpinBox->value() > kmaxSpinBox->value() || joinCheckBox->isChecked())
    kminSpinBox->setValue(kmaxSpinBox->value());

  binPlotter->setKMax(kmaxSpinBox->value());

  updateProbability();
  binPlotter->update();
}

void BinomialDistTab::updateProbability() {
  double v2 = 0;
  if(kminSpinBox->value())
    v2 = gsl_cdf_binomial_P(kminSpinBox->value()-1, pDSpinBox->value(), nSpinBox->value());

  lcdNumber->display(gsl_cdf_binomial_P(kmaxSpinBox->value(), pDSpinBox->value(), nSpinBox->value())-v2);
}

void BinomialDistTab::on_exportPushButton_clicked() {
  ExportDialog export_dialog(this, binPlotter, binPlotter->width(), binPlotter->height());
  export_dialog.exec();
}
