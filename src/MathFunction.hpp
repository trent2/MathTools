#ifndef _MATH_FUNCTION_HPP_
#define _MATH_FUNCTION_HPP_

class MathFunction {

public:
  MathFunction() { }
  ~MathFunction() { }

  double eval(double x) const {
    return x*x;
  }
};
#endif
