#ifndef _HYP_TEST_HPP_
#define _HYP_TEST_HPP_
#include "ui_hyptest.h"

class HypTestTab : public QWidget, private Ui::HypTestForm {

Q_OBJECT

public:
  HypTestTab(QWidget* parent=0);
private:
  int k1, k2;
private slots:  // slots are auto-connected
  void on_nSpinBox_valueChanged(int);
  void on_xSpinBox_valueChanged(int);
  void on_pDSpinBox_valueChanged(double);
  void on_alphaDSpinBox_valueChanged(double);

  void on_xmaxSpinBox_valueChanged(int);
  void on_yminDSpinBox_valueChanged(double);
  void on_ymaxDSpinBox_valueChanged(double);

  void on_hypPlotter_newXMax(double);
  void on_hypPlotter_newYMin(double);
  void on_hypPlotter_newYMax(double);

  void on_leftRButton_toggled(bool);
  void on_rightRButton_toggled(bool);
  void on_bothRButton_toggled(bool);

  void updateV();

  // export
  void on_exportPushButton_clicked();
};
#endif
