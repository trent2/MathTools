#ifndef _COMPLEX_TAB_HPP_
#define _COMPLEX_TAB_HPP_
#include "ui_complextab.h"

class ComplexTab : public QWidget, private Ui::ComplexPlotForm {

Q_OBJECT

public:
  ComplexTab(QWidget* parent=0);

private slots:  // slots are auto-connected
  void on_repaintPushButton_clicked();
  void on_standardPushButton_clicked();

  void on_plotter_newXMin(double xmin);
  void on_plotter_newXMax(double xmax);
  void on_plotter_newYMin(double ymin);
  void on_plotter_newYMax(double ymax);

  void on_xminDSpinBox_valueChanged(double);
  void on_xmaxDSpinBox_valueChanged(double);
  void on_yminDSpinBox_valueChanged(double);
  void on_ymaxDSpinBox_valueChanged(double);

  void on_rayThicknessDSpinBox_valueChanged(double);
  void on_rayOpacityDSpinBox_valueChanged(double);
  void on_circleThicknessDSpinBox_valueChanged(double);
  void on_circleOpacityDSpinBox_valueChanged(double);
  void on_infThresholdSpinBox_valueChanged(int);
};
#endif
