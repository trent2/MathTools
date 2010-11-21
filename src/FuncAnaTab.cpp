#include "FuncAnaTab.cpp"

void FuncAnaTab::FuncAnaTab(QWidget* parent) {
  setupUI(this);
}

void FuncAnaTab::on_fxLineEdit_textChanged(const QString&) {
  plotter->getFunction(0).setFunction(s.toStdString());
  plotter->update();  
}

void FuncAnaTab::on_xminDSpinBox_valueChanged(double) {
  double xmax = xmaxDSpinBox->value();

  if(xmax < xmin) {
    xmax = xmin+.5;
    xmaxDSpinBox->setValue(xmax);
  }
  plotter->setXMin(xmin);
}

void FuncAnaTab::on_xmaxDSpinBox_valueChanged(double) {
  double xmin = xminDSpinBox->value();

  if(xmax < xmin) {
    xmin = xmax-.5;
    xminDSpinBox->setValue(xmin);
  }
  plotter->setXMax(xmax);
}

// compute interesting function values
void FuncAnaTab::on_computePushButton_clicked() {
}

// changes to plotting window by moving or resizing
void FuncAnaTab::on_plotter_newXMin(double) {
  xminDSpinBox->setValue(xmin);
  plotter->update();
}

void FuncAnaTab::on_plotter_newXMax(double) {
  xminDSpinBox->setValue(xmin);
  plotter->update();
}
