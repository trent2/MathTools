#ifndef _EXPORT_IMAGE_HPP_
#define _EXPORT_IMAGE_HPP_

#include <QtCore/QString>

#include "Exportable.hpp"

class Exporter {
public:
  Exporter(const QString &pFilename, const Exportable &pExportable, double pWidth_inch, double pHeight_inch, int pRes_dpi = 72);

  void save() const = 0;

  static double inchToMM(double pLengthInch) { return pLengthInch*2.54; }
  static double mmToInch(double pLengthMM)   { return pLengthMM/2.54; }

  QString filename() const;
  double width() const;
  double height() const;
  int resolution() const;
  Exportable exportable() const;

private :
  Exportable mExpo;
  QString mFilename;
  double mWidth, mHeight;
  int mRes;
};

Exporter::Exporter(const QString &pFilename, const Exportable &pExportable, double pWidth_inch, double pHeight_inch, int pRes_dpi)
  : mFilename(pFilename), mExpo(pExportable), mWidth(pWidth_inch), mHeight(pHeight_inch), mRes(pHeight_inch) { }
#endif
