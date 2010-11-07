#ifndef _PLOTTERHELPER_HPP_
#define _PLOTTERHELPER_HPP_
#include <algorithm>
#include <sstream>
#include <iomanip>

#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtGui/QFontMetrics>

namespace PlotHelp {

  /*** Creates all number labels for an axis. If a QSize variable and QFontMetrics are given,
       the functions puts the size of the biggest box holding a number label on the axis into maxSize.
   **/
  void createAxisLabels(std::vector<QString> &ax_lab, double lmin, double lmax, double linc, QSize *maxSize = 0,
			const QFontMetrics *fm = 0, bool printZero = true) {
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
  /*** get the resolution ratio of the a paint device w.r.t. a reference paint device
   useful for computing line-width etc. when we want to specify all sizes w.r.t. a reference
   resolution (from the reference device) but need to adjust to different devices (for printing
   to a printer, file or an image file)
  **/
  qreal getResFactor(const QPaintDevice *actualPaintDevice, const QPaintDevice *referenceDevice) {
    return actualPaintDevice->logicalDpiX() / referenceDevice->logicalDpiX();
  }
}
#endif
