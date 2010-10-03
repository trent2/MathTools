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
};

#endif // __MATHTOOLS_HPP_
