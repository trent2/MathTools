#include <QtGui/QApplication>
#include "MathTools.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MathTools foo;
    foo.show();
    return app.exec();
}
