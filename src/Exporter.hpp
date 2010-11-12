#ifndef _EXPORTER_HPP_
#define _EXPORTER_HPP_
#include <QtCore/QString>

class Exportable;

// Strategy pattern
class Exporter {
public:
  virtual void save(const QString &pFilename, const Exportable *pExportable, double pWidth_inch, double pHeight_inch, int pRes_dpi = 72)
    const = 0;

protected:
  Exporter() { }
};


// subclasses of Exporter

class ExportPNG : public Exporter {
public :
  void save(const QString &pFilename, const Exportable *pExportable, double pWidth_inch, double pHeight_inch, int pRes_dpi = 72) const;
  static Exporter* getExportPNG();
private :
  static Exporter* exporter;
  ExportPNG() { }
};

class ExportPDF : public Exporter {
public :
  void save(const QString &pFilename, const Exportable *pExportable, double pWidth_inch, double pHeight_inch, int pRes_dpi = 72) const;
  static Exporter* getExportPDF();
private :
  static Exporter* exporter;
  ExportPDF() { }
};
#endif
