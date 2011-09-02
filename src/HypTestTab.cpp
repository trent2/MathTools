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

/*  file: --- HypTestTab.cpp --- */

#include <sstream>
#include <cmath>
#include <gsl/gsl_cdf.h>

#include "HypTestTab.hpp"
#include "ExportDialog.hpp"

HypTestTab::HypTestTab(QWidget* parent) : QWidget(parent) {
  setupUi(this);
  hypPlotter->setOptions(// Plotter::ZoomFixHorizontal | 
			 Plotter::AutoTicksX | Plotter::AutoTicksY);
  hypPlotter->setN(nSpinBox->value());
  hypPlotter->setP(pDSpinBox->value());
  hypPlotter->setTestResult(xSpinBox->value());
  hypPlotter->update();
  updateV();
}

void HypTestTab::on_nSpinBox_valueChanged(int n) {
  hypPlotter->setN(n);
  hypPlotter->setXMax(n);
  hypPlotter->setXMin(-1);
  hypPlotter->update();
  updateV();
}

void HypTestTab::on_xSpinBox_valueChanged(int tr) {
  hypPlotter->setTestResult(tr);
  updateV();
}

void HypTestTab::on_pDSpinBox_valueChanged(double p) {
  hypPlotter->setP(p);
  updateV();
  hypPlotter->update();
}

void HypTestTab::on_alphaDSpinBox_valueChanged(double) {
  updateV();
}

void HypTestTab::on_xmaxSpinBox_valueChanged(int xmax) {
  hypPlotter->setXMax(xmax);
}

void HypTestTab::on_yminDSpinBox_valueChanged(double ymin) {
  double ymax = ymaxDSpinBox->value();

  if(ymax < ymin) {
    ymax = ymin+.5;
    ymaxDSpinBox->setValue(ymax);
  }
  hypPlotter->setYMin(ymin);
}

void HypTestTab::on_ymaxDSpinBox_valueChanged(double ymax) {
  double ymin = yminDSpinBox->value();

  if(ymax < ymin) {
    ymin = ymax-.5;
    yminDSpinBox->setValue(ymin);
  }
  hypPlotter->setYMax(ymax);
}

void HypTestTab::on_hypPlotter_newXMax(double xmax) {
  if(hypPlotter->xMin()<-xmax*.05)
    hypPlotter->setXMin(-xmax*.05);
  xmaxSpinBox->setValue(xmax);

  hypPlotter->update();
}

void HypTestTab::on_hypPlotter_newYMin(double ymin) {
  yminDSpinBox->setValue(ymin);
  hypPlotter->update();
}

void HypTestTab::on_hypPlotter_newYMax(double ymax) {
  ymaxDSpinBox->setValue(ymax);
  hypPlotter->update();
}

void HypTestTab::updateV() {
  double m = nSpinBox->value()*pDSpinBox->value();
  int l = 0, h = nSpinBox->value();
  int k;
  bool rejected = true;
  double alpha = alphaDSpinBox->value();
  if(bothRButton->isChecked())
    alpha /= 2;
  std::ostringstream numstream, pstream;

  if(leftRButton->isChecked() || bothRButton->isChecked()) {  // test from left
    k=m/2;

    while(h-l>0) {
      if(gsl_cdf_binomial_P(k, pDSpinBox->value(), nSpinBox->value())>alpha)
	h = k-1;
      else
	l = k;
      k=std::ceil((h+l)/2.0);
    }
    k1 = h;
    hypPlotter->setKLow(k1);

    // create border probabilities string
    pstream << "<p>P(X&le;" << k1 << ") &asymp; " << gsl_cdf_binomial_P(k1, pDSpinBox->value(), nSpinBox->value()) << "</p>\n"
	    << "<p>P(X&le;" << k1+1 << ") &asymp; " << gsl_cdf_binomial_P(k1+1, pDSpinBox->value(), nSpinBox->value()) << "</p>\n";
  }

  l = 0, h = nSpinBox->value();
  if(rightRButton->isChecked() || bothRButton->isChecked()) {  // test from right
    k=3*m/2;
    while(h-l>0) {
      if(gsl_cdf_binomial_P(k-1, pDSpinBox->value(), nSpinBox->value())<1-alpha)
	l = k+1;
      else
	h = k;
      k=std::floor((h+l)/2.0);
    }
    k2 = h;
    hypPlotter->setKHigh(k2);

    // create border probabilities string
    pstream << "<p>P(X&ge;" << k2 << ") &asymp; " << 1-gsl_cdf_binomial_P(k2-1, pDSpinBox->value(), nSpinBox->value()) << "</p>\n"
	    << "<p>P(X&ge;" << k2-1 << ") &asymp; " << 1-gsl_cdf_binomial_P(k2-2, pDSpinBox->value(), nSpinBox->value()) << "</p>";
  }

  // create region of rejection string
  if(leftRButton->isChecked()) {
    numstream << "{" << 0 << ", ..., " << k1 << "}";
    rejected = xSpinBox->value() <= k1;
  }
  else if(rightRButton->isChecked()) {
    numstream << "{" << k2 << ", ..., " << nSpinBox->value() << "}";
    rejected = xSpinBox->value() >= k2;
  }
  else if(bothRButton->isChecked()) {
    numstream << "{" << 0 << ", ..., " << k1 << ", " << k2 << ", ..., " << nSpinBox->value() << "}";
    rejected = xSpinBox->value() <= k1 || xSpinBox->value() >= k2;
  }

  // output strings
  pTextBrowser->setText(pstream.str().c_str());
  vLineEdit->setText(numstream.str().c_str());
  if(rejected)
    acceptTextBrowser->setHtml(tr("<span style=\"color: red; font-size: 20pt; \"<b>H<sub>0</sub> rejected</b></span>"));
  else
    acceptTextBrowser->setHtml(tr("<span style=\"color: green; font-size: 20pt; \"<b>H<sub>0</sub> accepted</b></span>"));

  hypPlotter->update();
}


void HypTestTab::on_leftRButton_toggled(bool) {
  hypPlotter->setTest(HypTestPlotter::left);
  updateV();
}

void HypTestTab::on_rightRButton_toggled(bool) {
  hypPlotter->setTest(HypTestPlotter::right);
  updateV();
}

void HypTestTab::on_bothRButton_toggled(bool) {
  hypPlotter->setTest(HypTestPlotter::both);
  updateV();
}

void HypTestTab::on_exportPushButton_clicked() {
  ExportDialog export_dialog(this, hypPlotter, hypPlotter->width(), hypPlotter->height());
  export_dialog.exec();
}
