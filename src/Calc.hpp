#ifndef __CALC_HPP_
#define __CALC_HPP_

#include <cmath>

namespace calc {
  inline int roundi(double d) {
    return d<0 ? d-0.5 : d+0.5;
  }

  inline double round(double d) {
    return d<0 ? std::ceil(d-0.5) : std::floor(d+0.5);
  }
}
#endif
