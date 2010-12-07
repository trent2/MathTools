#ifndef __MATHTOOLS_HPP_
#define __MATHTOOLS_HPP_

#include "ui_mathtoolsmain.h"

class QMainWindow;

class MathTools : public QMainWindow, private Ui::MathToolsMainWindow
{

Q_OBJECT

public:
  MathTools(QWidget * parent = 0, Qt::WFlags f = Qt::Window);

private slots:  // slots are auto-connected
  void on_plotAction_triggered();
  void on_binomialDistAction_triggered();
  void on_testingAction_triggered();
};

#endif // __MATHTOOLS_HPP_
