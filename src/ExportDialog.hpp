#ifndef _EXPORT_DIALOG_HPP_
#define _EXPORT_DIALOG_HPP_
#include "ui_exportdialog.h"

class Exportable;

class ExportDialog : public QDialog, public Ui::ExportDialog {

Q_OBJECT

public:
  enum OutputFormat {pdf, png};
  enum Measure {cm, in};
  enum ResolutionRatio {in_px, cm_px};

  ExportDialog(QWidget* parent, Exportable *expo, double width, double height,
	       OutputFormat format = pdf, Measure m = cm, ResolutionRatio rr = in_px);

private:
  Exportable *mExp;
  double mWidth, mHeight;
  Measure mMeas;
  ResolutionRatio mResType;
  double mRes;

  static const int minInchResRatio;
  static const int maxInchResRatio;

  static const int minMMSize;
  static const int maxMMSize;

private slots:
  void on_buttonBox_accepted();
};
#endif
