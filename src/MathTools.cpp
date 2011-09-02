#include <QtGui/QMainWindow>

#include "HypTestTab.hpp"
#include "BinomialDistTab.hpp"
#include "PlotTab.hpp"
#include "ComplexTab.hpp"
#include "MathTools.hpp"
#include "About.hpp"

MathTools::MathTools(QWidget *parent, Qt::WFlags f) : QMainWindow(parent, f)
{
  setupUi(this);
}

void MathTools::on_plotAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new PlotTab(mainTabWidget), "Plot"));
}

void MathTools::on_complexPlotAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new ComplexTab(mainTabWidget), "Complex Plot"));
}

void MathTools::on_binomialDistAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new BinomialDistTab(mainTabWidget), "Binomial Distribution"));
}

void MathTools::on_testingAction_triggered() {
  mainTabWidget->setCurrentIndex(mainTabWidget->addTab(new HypTestTab(mainTabWidget), "Testing"));
}

void MathTools::on_aboutAction_triggered() {
  AboutDialog(this).exec();
}
