#ifndef _BIN_DIST_TAB_HPP_
#define _BIN_DIST_TAB_HPP_
#include "ui_binomialdist.h"

class BinomialDistTab : public QWidget, private Ui::BinomialDistForm {

Q_OBJECT

public:
  BinomialDistTab(QWidget* parent=0);

private slots:  // slots are auto-connected
  void on_nSpinBox_valueChanged(int);
  void on_pDSpinBox_valueChanged(double);

  void on_xmaxSpinBox_valueChanged(int);
  void on_yminDSpinBox_valueChanged(double);
  void on_ymaxDSpinBox_valueChanged(double);

  void on_binPlotter_newXMax(double);
  void on_binPlotter_newYMin(double);
  void on_binPlotter_newYMax(double);

  void on_distRButton_toggled(bool);

  void on_joinCheckBox_toggled(bool);

  void on_kminSpinBox_valueChanged(int);
  void on_kmaxSpinBox_valueChanged(int);

  void updateProbability();

  // export
  void on_exportPushButton_clicked();

};
#endif
