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

/*  file: --- PlotterHelper.hpp --- */

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
