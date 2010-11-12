#ifndef _EXPORTABLE_HPP_
#define _EXPORTABLE_HPP_

#include <QtGui/QPainter>

struct Exportable {
  virtual void paintIt(QPaintDevice*, QPainter::RenderHints = 0) const = 0;
};
#endif
