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

/*  file: --- Exporter.cpp --- */

#include <QtGui/QImage>
#include <QtGui/QPrinter>
#include <QtGui/QPainter>

#include "Exportable.hpp"
#include "Exporter.hpp"

Exporter* ExportPNG::exporter = 0;
Exporter* ExportPDF::exporter = 0;

Exporter* ExportPDF::getExportPDF() {
  if(!exporter)
    exporter = new ExportPDF();
  return exporter;
}

Exporter* ExportPNG::getExportPNG() {
  if(!exporter)
    exporter = new ExportPNG();
  return exporter;
}

void ExportPDF::save(const QString &pFilename, const Exportable *pExportable, double pWidth_inch, double pHeight_inch, int pRes_dpi) const {
  QPrinter printer; // (QPrinter::HighResolution);
  printer.setPaperSize(QSizeF(pWidth_inch, pHeight_inch), QPrinter::Inch);
  printer.setPageMargins(0, 0, 0, 0, QPrinter::Inch);
  printer.setResolution(pRes_dpi);

  printer.setCreator("MathTools");
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setOutputFileName(pFilename);
  pExportable->paintIt(&printer, QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
}

void ExportPNG::save(const QString &pFilename, const Exportable *pExportable, double pWidth_inch, double pHeight_inch, int pRes_dpi) const {
  QImage printer(pWidth_inch*pRes_dpi, pHeight_inch*pRes_dpi, QImage::Format_RGB888);
  printer.setDotsPerMeterX(pRes_dpi/2.54*100);
  printer.setDotsPerMeterY(pRes_dpi/2.54*100);
  printer.fill(0xffffff);

  pExportable->paintIt(&printer, QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
  printer.save(pFilename);
}
