#include <QFileDialog>

#include "ExportDialog.hpp"
#include "Exporter.hpp"
#include "Exportable.hpp"

const int ExportDialog::minInchResRatio = 30;
const int ExportDialog::maxInchResRatio = 7200;

const int ExportDialog::minMMSize = 0.01;
const int ExportDialog::maxMMSize = 400;

//  default: OutputFormat format = pdf, Measure m = cm,
//           ResolutionRatio rr = in_px
ExportDialog::ExportDialog(QWidget* parent, Exportable *expo, double width, double height,
			   OutputFormat format, Measure m, ResolutionRatio rr) : QDialog(parent),
					       mExp(expo), mWidth(width), mHeight(height)
{

  setupUi(this);

  mRes = physicalDpiX();

  mWidth /= mRes;
  mHeight /= mRes;

  if(m==cm) {
    mWidth  *= 2.54;
    mHeight *= 2.54;
  }
  if(rr==cm_px)
    mRes /= 2.54;

  outputFormatComboBox->setCurrentIndex(format);
  measureComboBox->setCurrentIndex(m);
  resRatioComboBox->setCurrentIndex(rr);
  resSpinBox->setValue(mRes);
  widthSpinBox->setValue(mWidth);
  heightSpinBox->setValue(mHeight);
}


void ExportDialog::on_buttonBox_accepted() {
  QString filter;
  switch(outputFormatComboBox->currentIndex()) {
  case ExportDialog::pdf: filter = "PDF-Files (*.pdf)"; break;
  case ExportDialog::png: filter = "PNG-Files (*.png)"; break;
  }
  QString filename = QFileDialog::getSaveFileName(this, "Export to filename...", QDir::currentPath(), filter);
  if(filename.isEmpty())
    return;

  // extract values from export_dialog
  double width = widthSpinBox->value(),
    height = heightSpinBox->value();
  if(measureComboBox->currentIndex()==ExportDialog::cm) {
    width  /= 2.54;
    height /= 2.54;
  };

  int res_dpi = resSpinBox->value();
  if(resRatioComboBox->currentIndex()==ExportDialog::cm_px)
    res_dpi /= 2.54; 

  // export to file
  Exporter* exporter;
  if(outputFormatComboBox->currentIndex() == ExportDialog::pdf)
    exporter = ExportPDF::getExportPDF();
  else
    exporter = ExportPNG::getExportPNG();

  exporter->save(filename, mExp, width, height, res_dpi);
}
