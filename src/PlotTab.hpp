#ifndef _PLOT_TAB_HPP_
#define _PLOT_TAB_HPP_
#include "ui_plottab.h"

class PlotTab : public QWidget, private Ui::PlotForm {

Q_OBJECT

public:
  PlotTab(QWidget* parent=0);

private slots:  // slots are auto-connected
  void on_f1LineEdit_textChanged(const QString&);
  void on_f2LineEdit_textChanged(const QString&);
  void on_f3LineEdit_textChanged(const QString&);
  void on_f4LineEdit_textChanged(const QString&);

  void on_f1CCombo_currentIndexChanged(int);
  void on_f2CCombo_currentIndexChanged(int);
  void on_f3CCombo_currentIndexChanged(int);
  void on_f4CCombo_currentIndexChanged(int);
  
  void on_xminDSpinBox_valueChanged(double);
  void on_xmaxDSpinBox_valueChanged(double);
  void on_yminDSpinBox_valueChanged(double);
  void on_ymaxDSpinBox_valueChanged(double);

  void on_xticksDSpinBox_valueChanged(double);
  void on_yticksDSpinBox_valueChanged(double);
  void on_xAutoCheckBox_toggled(bool);
  void on_yAutoCheckBox_toggled(bool);
  void on_gridXCheckBox_toggled(bool);
  void on_gridYCheckBox_toggled(bool);
  void on_gridBothCheckBox_toggled(bool);
  // zoom to standard
  void on_standardPushButton_clicked();

  // changes to plotting window by moving or resizing
  void on_plotter_newXMin(double);
  void on_plotter_newXMax(double);
  void on_plotter_newYMin(double);
  void on_plotter_newYMax(double);

  // new ticks from auto tick
  void on_plotter_newXTicks(double);
  void on_plotter_newYTicks(double);

  // export
  void on_exportPushButton_clicked();
};
#endif
