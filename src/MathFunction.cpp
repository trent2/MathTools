#include "MathFunction.hpp"
#include "parser.hpp"

MathFunction::MathFunction(const std::string &s) : func_string(s) {
}

bool MathFunction::can_eval() const {
  return parser::arith_parser.valid(func_string);
}

double MathFunction::eval(const double &d) const {
  return parser::arith_parser.eval(d, func_string);
}
