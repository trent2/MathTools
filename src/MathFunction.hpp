#ifndef __MATH_FUNCTION_HPP__
#define __MATH_FUNCTION_HPP__

#include <string>

class MathFunction {

public:
  MathFunction() : func_string("0") { }
  MathFunction(const std::string &);

  ~MathFunction() { }

  void setFunction(const std::string &s) {
    func_string = s;
  }

  bool can_eval() const;

  double eval(const double &) const;

private:
  std::string func_string;
};
#endif
