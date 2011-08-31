#ifndef __ABOUT_HPP_
#define __ABOUT_HPP_

#include "ui_about.h"

class AboutDialog : public QDialog, private Ui::AboutDialog {

Q_OBJECT

public:
  AboutDialog(QWidget* parent=0) : QDialog(parent) { setupUi(this); }
};
#endif
