#include <QPrinter>

#include "PlotTab.hpp"
#include "MathFunction.hpp"

PlotTab::PlotTab(QWidget* parent) : QWidget(parent) {
  setupUi(this);
  f1CCombo->setStandardColors();
  f1CCombo->setCurrentColor(Qt::red);
  f2CCombo->setStandardColors();
  f2CCombo->setCurrentColor(Qt::darkBlue);
  f3CCombo->setStandardColors();
  f3CCombo->setCurrentColor(Qt::magenta);
  f4CCombo->setStandardColors();
  f4CCombo->setCurrentColor(Qt::yellow);
}

/* PlotTab slots */

void PlotTab::on_f1LineEdit_textChanged(const QString &s) {
  plotter->getFunction(0).setFunction(s.toStdString());
  plotter->update();
}

void PlotTab::on_f2LineEdit_textChanged(const QString &s) {
  plotter->getFunction(1).setFunction(s.toStdString());
  plotter->update();
}

void PlotTab::on_f3LineEdit_textChanged(const QString &s) {
  plotter->getFunction(2).setFunction(s.toStdString());
  plotter->update();
}

void PlotTab::on_f4LineEdit_textChanged(const QString &s) {
  plotter->getFunction(3).setFunction(s.toStdString());
  plotter->update();
}

void PlotTab::on_f1CCombo_currentIndexChanged(int) {
  plotter->getFunction(0).setColor(f1CCombo->currentColor());
  plotter->update();
}

void PlotTab::on_f2CCombo_currentIndexChanged(int) {
  plotter->getFunction(1).setColor(f2CCombo->currentColor());
  plotter->update();
}

void PlotTab::on_f3CCombo_currentIndexChanged(int) {
  plotter->getFunction(2).setColor(f3CCombo->currentColor());
  plotter->update();
}

void PlotTab::on_f4CCombo_currentIndexChanged(int) {
  plotter->getFunction(3).setColor(f4CCombo->currentColor());
  plotter->update();
}

void PlotTab::on_xminDSpinBox_valueChanged(double xmin) {
  double xmax = xmaxDSpinBox->value();

  if(xmax < xmin) {
    xmax = xmin+.5;
    xmaxDSpinBox->setValue(xmax);
  }
  plotter->xmin = xmin;
  plotter->update();
}

void PlotTab::on_xmaxDSpinBox_valueChanged(double xmax) {
  double xmin = xminDSpinBox->value();

  if(xmax < xmin) {
    xmin = xmax-.5;
    xminDSpinBox->setValue(xmin);
  }
  plotter->xmax = xmax;
  plotter->update();
}

void PlotTab::on_yminDSpinBox_valueChanged(double ymin) {
  double ymax = ymaxDSpinBox->value();

  if(ymax < ymin) {
    ymax = ymin+.5;
    ymaxDSpinBox->setValue(ymax);
  }
  plotter->ymin = ymin;
  plotter->update();
}

void PlotTab::on_ymaxDSpinBox_valueChanged(double ymax) {
  double ymin = yminDSpinBox->value();

  if(ymax < ymin) {
    ymin = ymax-.5;
    yminDSpinBox->setValue(ymin);
  }
  plotter->ymax = ymax;
  plotter->update();
}

void PlotTab::on_xticksDSpinBox_valueChanged(double xticks) {
  plotter->setXTicks(xticks);
  plotter->update();
}

void PlotTab::on_yticksDSpinBox_valueChanged(double yticks) {
  plotter->setYTicks(yticks);
  plotter->update();
}

void PlotTab::on_xAutoCheckBox_toggled(bool b) {
  plotter->compAutoXTicks = b;
  plotter->update();
}

void PlotTab::on_yAutoCheckBox_toggled(bool b) {
  plotter->compAutoYTicks = b;
  plotter->update();
}

void PlotTab::on_gridCheckBox_toggled(bool b) {
  plotter->drawGrid = b;
  plotter->update();
}

void PlotTab::on_standardPushButton_clicked() {
  this->on_xminDSpinBox_valueChanged(-10);
  this->on_xmaxDSpinBox_valueChanged(10);
  this->on_yminDSpinBox_valueChanged(-10);
  this->on_ymaxDSpinBox_valueChanged(10);
}

void PlotTab::on_plotter_newXMin(double xmin) {
  xminDSpinBox->setValue(xmin);
}

void PlotTab::on_plotter_newXMax(double xmax) {
  xmaxDSpinBox->setValue(xmax);
}

void PlotTab::on_plotter_newYMin(double ymin) {
  yminDSpinBox->setValue(ymin);
}

void PlotTab::on_plotter_newYMax(double ymax) {
  ymaxDSpinBox->setValue(ymax);
}

void PlotTab::on_plotter_newXTicks(double xticks) {
  xticksDSpinBox->setValue(xticks);
}

void PlotTab::on_plotter_newYTicks(double yticks) {
  yticksDSpinBox->setValue(yticks);
}

void PlotTab::on_exportPushButton_clicked() {
  QPrinter printer; // (QPrinter::HighResolution);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setOutputFileName("test.pdf");
  plotter->paintIt(&printer);
}
