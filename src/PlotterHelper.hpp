#ifndef _PLOTTERHELPER_HPP_
#define _PLOTTERHELPER_HPP_
#include <vector>
#include <QtCore/QString>
#include <QtCore/QSize>
#include <QtGui/QFontMetrics>

namespace PlotHelp {

  struct old_vals_t {
    double xmin, xmax, ymin, ymax;
    // mean(xmax,xmin), mean(ymax,ymin)
    double anchorMX, anchorMY;
    // position of mouse move start (in pixel coords)
    int anchorWX, anchorWY;
  };

  struct cs_params {
    int winwidth, winheight;
    // pixels per unit
    double xstep, ystep;
    // distance between number label on axes
    double descDistX, descDistY;
  };

  
  /***
   *  Creates all number labels for an axis. If a QSize variable and QFontMetrics are given,
   *  the functions puts the size of the biggest box holding a number label on the axis into maxSize.
   **/
  void createAxisLabels(std::vector<QString> &ax_lab, double lmin, double lmax, double linc,
			       QSize *maxSize = 0, const QFontMetrics *fm = 0, bool printZero = true);

  /*** get the resolution ratio of the a paint device w.r.t. a reference paint device
   useful for computing line-width etc. when we want to specify all sizes w.r.t. a reference
   resolution (from the reference device) but need to adjust to different devices (for printing
   to a printer, file or an image file)
  **/
  qreal getResFactor(const QPaintDevice *actualPaintDevice, const QPaintDevice *referenceDevice);
}
#endif
