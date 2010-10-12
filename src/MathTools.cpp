#include <QtGui/QMainWindow>

#include "MathTools.hpp"
#include "MathFunction.hpp"

MathTools::MathTools(QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
  setupUi(this);
  f1CCombo->setStandardColors();
  f1CCombo->setCurrentColor(Qt::red);
  f2CCombo->setStandardColors();
  f2CCombo->setCurrentColor(Qt::darkBlue);
  f3CCombo->setStandardColors();
  f3CCombo->setCurrentColor(Qt::magenta);
  f4CCombo->setStandardColors();
  f4CCombo->setCurrentColor(Qt::yellow);
}

MathTools::~MathTools()
{}


void MathTools::setF1(const QString &s) {
  plotter->getFunction(0).setFunction(s.toStdString());
  update();
}

void MathTools::setF2(const QString &s) {
  plotter->getFunction(1).setFunction(s.toStdString());
  update();
}

void MathTools::setF3(const QString &s) {
  plotter->getFunction(2).setFunction(s.toStdString());
  update();
}

void MathTools::setF4(const QString &s) {
  plotter->getFunction(3).setFunction(s.toStdString());
  update();
}

void MathTools::setF1Color() {
  plotter->getFunction(0).setColor(f1CCombo->currentColor());
  update();
}

void MathTools::setF2Color() {
  plotter->getFunction(1).setColor(f2CCombo->currentColor());
  update();
}

void MathTools::setF3Color() {
  plotter->getFunction(2).setColor(f3CCombo->currentColor());
  update();
}

void MathTools::setF4Color() {
  plotter->getFunction(3).setColor(f4CCombo->currentColor());
  update();
}
