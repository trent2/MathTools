#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QTextCodec>

#include "MathTools.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MathTools foo;

    QTranslator translator;
    translator.load("mathtools_" + QLocale::system().name());
    app.installTranslator(&translator);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    foo.show();
    return app.exec();
}
