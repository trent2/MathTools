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
