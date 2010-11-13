#include <QFileDialog>

#include "PlotTab.hpp"
#include "MathFunction.hpp"
#include "ExportDialog.hpp"

#include "Exporter.hpp"

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
  plotter->setXMin(xmin);
}

void PlotTab::on_xmaxDSpinBox_valueChanged(double xmax) {
  double xmin = xminDSpinBox->value();

  if(xmax < xmin) {
    xmin = xmax-.5;
    xminDSpinBox->setValue(xmin);
  }
  plotter->setXMax(xmax);
}

void PlotTab::on_yminDSpinBox_valueChanged(double ymin) {
  double ymax = ymaxDSpinBox->value();

  if(ymax < ymin) {
    ymax = ymin+.5;
    ymaxDSpinBox->setValue(ymax);
  }
  plotter->setYMin(ymin);
}

void PlotTab::on_ymaxDSpinBox_valueChanged(double ymax) {
  double ymin = yminDSpinBox->value();

  if(ymax < ymin) {
    ymin = ymax-.5;
    yminDSpinBox->setValue(ymin);
  }
  plotter->setYMax(ymax);
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
  plotter->setCompAutoXTicks(b);
  plotter->update();
}

void PlotTab::on_yAutoCheckBox_toggled(bool b) {
  plotter->setCompAutoYTicks(b);
  plotter->update();
}

void PlotTab::on_gridXCheckBox_toggled(bool b) {
  plotter->setDrawGridX(b);
  if(!b)
    gridBothCheckBox->setChecked(false);
  plotter->update();
}

void PlotTab::on_gridYCheckBox_toggled(bool b) {
  plotter->setDrawGridY(b);
  if(!b)
    gridBothCheckBox->setChecked(false);
  plotter->update();
}

void PlotTab::on_gridBothCheckBox_toggled(bool b) {
  if(b) {
    gridXCheckBox->setChecked(true);
    gridYCheckBox->setChecked(true);
  }
  plotter->update();
}

void PlotTab::on_standardPushButton_clicked() {
  plotter->setXMin(-10);
  plotter->setXMax(10);
  plotter->setYMin(-10);
  plotter->setYMax(10);
}

void PlotTab::on_plotter_newXMin(double xmin) {
  xminDSpinBox->setValue(xmin);
  plotter->update();
}

void PlotTab::on_plotter_newXMax(double xmax) {
  xmaxDSpinBox->setValue(xmax);
  plotter->update();
}

void PlotTab::on_plotter_newYMin(double ymin) {
  yminDSpinBox->setValue(ymin);
  plotter->update();
}

void PlotTab::on_plotter_newYMax(double ymax) {
  ymaxDSpinBox->setValue(ymax);
  plotter->update();
}

void PlotTab::on_plotter_newXTicks(double xticks) {
  xticksDSpinBox->setValue(xticks);
}

void PlotTab::on_plotter_newYTicks(double yticks) {
  yticksDSpinBox->setValue(yticks);
}

void PlotTab::on_exportPushButton_clicked() {
  ExportDialog export_dialog(plotter->width(), plotter->height(), this);
  int ret = export_dialog.exec();

  if(ret == QDialog::Rejected)
    return;

  // Accepted
  QString filter;
  switch(export_dialog.outputFormatComboBox->currentIndex()) {
  case ExportDialog::pdf: filter = "PDF-Files (*.pdf)"; break;
  case ExportDialog::png: filter = "PNG-Files (*.png)"; break;
  }
  QString filename = QFileDialog::getSaveFileName(this, "Export to filename...", QDir::currentPath(), filter);
  if(filename.isEmpty())
    return;

  // extract values from export_dialog
  double width = export_dialog.widthSpinBox->value(),
    height = export_dialog.heightSpinBox->value();
  if(export_dialog.measureComboBox->currentIndex()==ExportDialog::cm) {
    width  /= 2.54;
    height /= 2.54;
  };

  int res_dpi = export_dialog.resSpinBox->value();
  if(export_dialog.resRatioComboBox->currentIndex()==ExportDialog::cm_px)
    res_dpi /= 2.54; 

  // export to file
  Exporter* exporter;
  if(export_dialog.outputFormatComboBox->currentIndex() == ExportDialog::pdf)
    exporter = ExportPDF::getExportPDF();
  else
    exporter = ExportPNG::getExportPNG();

  exporter->save(filename, plotter, width, height, res_dpi);
}
