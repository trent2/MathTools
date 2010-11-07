#include "ExportDialog.hpp"

const int ExportDialog::minInchResRatio = 30;
const int ExportDialog::maxInchResRatio = 7200;

const int ExportDialog::minMMSize = 0.01;
const int ExportDialog::maxMMSize = 400;

//  default: OutputFormat format = pdf, Measure m = cm,
//           ResolutionRatio rr = in_px
ExportDialog::ExportDialog(double width, double height, QWidget* parent, OutputFormat format,
			   Measure m, ResolutionRatio rr) : QDialog(parent),
							    mWidth(width), mHeight(height)
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

/*
void ExportDialog::on_exportDialog_accept() {
  QString filter;
  switch(outputFormatComboBox->currentIndex()) {
  case pdf: filter = "PDF-Files (*.pdf)"; break;
  case png: filter = "PNG-Files (*.png)"; break;
  }
  filenameLineEdit->setText(QFileDialog::getSaveFileName(this, "Export to filename...", QDir::currentPath(), filter));
}
*/
