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

void HypTestTab::on_xSpinBox_valueChanged(int) {
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
  std::ostringstream numstream;

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
  }

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
  
  vLineEdit->setText(numstream.str().c_str());
  if(rejected)
    acceptTextBrowser->setHtml("<span style=\"color: red; font-size: 20pt; \"<b>H<sub>0</sub> rejected</b></span>");
  else
    acceptTextBrowser->setHtml("<span style=\"color: green; font-size: 20pt; \"<b>H<sub>0</sub> accepted</b></span>");
}


void HypTestTab::on_leftRButton_toggled(bool) {
  updateV();
}

void HypTestTab::on_rightRButton_toggled(bool) {
  updateV();
}

void HypTestTab::on_bothRButton_toggled(bool) {
  updateV();
}

void HypTestTab::on_exportPushButton_clicked() {
  ExportDialog export_dialog(this, hypPlotter, hypPlotter->width(), hypPlotter->height());
  export_dialog.exec();
}
