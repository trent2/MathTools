#ifndef __MATHTOOLS_HPP_
#define __MATHTOOLS_HPP_

#include "ui_mathtools.h"

class QMainWindow;

class MathTools : public QMainWindow, public Ui::MathTools
{
Q_OBJECT
public:
  MathTools(QWidget * parent = 0, Qt::WFlags f = Qt::Window);
  virtual ~MathTools();

public slots:
  void setF1(const QString&);
  void setF2(const QString&);
  void setF3(const QString&);
  void setF4(const QString&);

  void setF1Color();
  void setF2Color();
  void setF3Color();
  void setF4Color();
};

#endif // __MATHTOOLS_HPP_
