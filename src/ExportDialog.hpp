#ifndef _EXPORT_DIALOG_HPP_
#define _EXPORT_DIALOG_HPP_
#include "ui_exportdialog.h"

class ExportDialog : public QDialog, public Ui::ExportDialog {

Q_OBJECT

public:
  enum OutputFormat {pdf, png};
  enum Measure {cm, in};
  enum ResolutionRatio {in_px, cm_px};

  ExportDialog(double width, double height, QWidget* parent, OutputFormat format = pdf,
	       Measure m = cm, ResolutionRatio rr = in_px);

private:
  double mWidth, mHeight;
  Measure mMeas;
  ResolutionRatio mResType;
  double mRes;

  static const int minInchResRatio;
  static const int maxInchResRatio;

  static const int minMMSize;
  static const int maxMMSize;

  // private slots:
  //  void on_exportDialog_accept();
};
#endif
