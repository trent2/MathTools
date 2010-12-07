#include <QtGui/QMainWindow>

#include "HypTestTab.hpp"
#include "BinomialDistTab.hpp"
#include "PlotTab.hpp"
#include "MathTools.hpp"

MathTools::MathTools(QWidget *parent, Qt::WFlags f) : QMainWindow(parent, f)
{
  setupUi(this);
}

void MathTools::on_plotAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new PlotTab(mainTabWidget), "Plot"));
}

void MathTools::on_binomialDistAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new BinomialDistTab(mainTabWidget), "Binomial Distribution"));
}

void MathTools::on_testingAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new HypTestTab(mainTabWidget), "Testing"));
}
