#include "ComplexTab.hpp"

ComplexTab::ComplexTab(QWidget* parent) : QWidget(parent) {
  setupUi(this);
}

/* PlotTab slots */

void ComplexTab::on_repaintPushButton_clicked() {
  plotter->getFunction().setFunction(fLineEdit->text().toStdString());
  plotter->doARepaint();
}

void ComplexTab::on_plotter_newXMin(double xmin) {
  xminDSpinBox->setValue(xmin);
  plotter->update();
}

void ComplexTab::on_plotter_newXMax(double xmax) {
  xmaxDSpinBox->setValue(xmax);
  plotter->update();
}

void ComplexTab::on_plotter_newYMin(double ymin) {
  yminDSpinBox->setValue(ymin);
  plotter->update();
}

void ComplexTab::on_plotter_newYMax(double ymax) {
  ymaxDSpinBox->setValue(ymax);
  plotter->update();
}

void ComplexTab::on_xminDSpinBox_valueChanged(double xmin){
  double xmax = xmaxDSpinBox->value();

  if(xmax < xmin) {
    xmax = xmin+.5;
    xmaxDSpinBox->setValue(xmax);
  }
  plotter->setXMin(xmin);
}

void ComplexTab::on_xmaxDSpinBox_valueChanged(double xmax){
  double xmin = xminDSpinBox->value();

  if(xmax < xmin) {
    xmin = xmax-.5;
    xminDSpinBox->setValue(xmin);
  }
  plotter->setXMax(xmax);
}

void ComplexTab::on_yminDSpinBox_valueChanged(double ymin){
  double ymax = ymaxDSpinBox->value();

  if(ymax < ymin) {
    ymax = ymin+.5;
    ymaxDSpinBox->setValue(ymax);
  }
  plotter->setYMin(ymin);
}

void ComplexTab::on_ymaxDSpinBox_valueChanged(double ymax){
  double ymin = yminDSpinBox->value();

  if(ymax < ymin) {
    ymin = ymax-.5;
    yminDSpinBox->setValue(ymin);
  }
  plotter->setYMax(ymax);
}


void ComplexTab::on_rayThicknessDSpinBox_valueChanged(double d) {
  plotter->setRayThickness(d);
}

void ComplexTab::on_rayOpacityDSpinBox_valueChanged(double d) {
  plotter->setRayOpacity(d);
}

void ComplexTab::on_circleThicknessDSpinBox_valueChanged(double d) {
  plotter->setCircleThickness(d);
}

void ComplexTab::on_circleOpacityDSpinBox_valueChanged(double d) {
  plotter->setCircleOpacity(d);
}

void ComplexTab::on_infThresholdSpinBox_valueChanged(int i) {
  plotter->setInfinityThreshold(i);
}

void ComplexTab::on_standardPushButton_clicked() {
  plotter->setXMin(-2);
  plotter->setXMax(2);
  plotter->setYMin(-2);
  plotter->setYMax(2);
}
