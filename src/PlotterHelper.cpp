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
