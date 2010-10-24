#include <QtGui/QMainWindow>

#include "PlotTab.hpp"
#include "MathTools.hpp"

MathTools::MathTools(QWidget *parent, Qt::WFlags f) : QMainWindow(parent, f)
{
  setupUi(this);
}

void MathTools::on_plotAction_triggered() {
  mainTabWidget->addTab(new PlotTab(mainTabWidget), "Plot");
}
