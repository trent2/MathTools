#ifndef __CALC_HPP_
#define __CALC_HPP_

#include <cmath>

int roundi(double d) {
  return d<0 ? d-0.5 : d+0.5;
}

double round(double d) {
  return d<0 ? std::ceil(d-0.5) : std::floor(d+0.5);
}

template <typename T>
T sign(T t) { return (t < 0) ? T(-1) : T(1); }
#endif