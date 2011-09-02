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

/*  file: --- PlotterHelper.cpp --- */

#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath>

#include <QtGui/QPaintDevice>

#include "PlotterHelper.hpp"

namespace PlotHelp {
  void createAxisLabels(std::vector<QString> &ax_lab, double lmin, double lmax, double linc, QSize *maxSize,
			const QFontMetrics *fm, bool printZero) {
    std::ostringstream numberstream;
    numberstream << std::setprecision(4) << std::noshowpoint;
    QString s;
    QSize numSize;
    if(maxSize && fm) {
      maxSize->setWidth(0);
      maxSize->setHeight(0);
    }

    while(lmin <= lmax) {
      if(std::abs(lmin) < 32*std::numeric_limits<double>::epsilon()) {
	lmin = 0;
      }

      if(printZero || lmin) {
	numberstream.str("");
	numberstream << lmin;
	s = numberstream.str().c_str();
	ax_lab.push_back(s);

	// adjust maximum size box
	if(maxSize && fm) {
	  numSize = fm->size(Qt::TextSingleLine, s);
	  maxSize->setWidth(std::max(numSize.width(), maxSize->width()));
	  maxSize->setHeight(std::max(numSize.height(), maxSize->height()));
	}
      } else
	ax_lab.push_back("");

      lmin += linc;
    }
  }

  qreal getResFactor(const QPaintDevice *actualPaintDevice, const QPaintDevice *referenceDevice) {
    return actualPaintDevice->logicalDpiX() / referenceDevice->logicalDpiX();
  }

}
