#ifndef _FUNC_ANA_TAB_HPP_
#define _FUNC_ANA_TAB_HPP_
#include "ui_plottab.h"

class FuncAnaTab : public QWidget, private Ui::AnalysisForm {

Q_OBJECT

public:
  FuncAnaTab(QWidget* parent=0);

private slots:  // slots are auto-connected
  void on_fxLineEdit_textChanged(const QString&);

  void on_xminDSpinBox_valueChanged(double);
  void on_xmaxDSpinBox_valueChanged(double);

  // compute interesting function values
  void on_computePushButton_clicked();

  // changes to plotting window by moving or resizing
  void on_plotter_newXMin(double);
  void on_plotter_newXMax(double);
};
#endif
