/*  MathTools 
    Copyright (C) 2010, 2011, 2012 Robert Spillner <Robert.Spillner@gmx.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*  file: --- main.cpp --- */

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
