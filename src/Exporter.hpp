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

/*  file: --- Exporter.hpp --- */

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
