#include "MathFunction.hpp"
#include "parser.hpp"

MathFunction::MathFunction(const QColor &col) : func_string(""), f(0), parser(new parser::unary_function_parser), color(col) { }

MathFunction::MathFunction(const std::string &s, const QColor &col) : func_string(s), f(0),
								      parser(new parser::unary_function_parser), color(col) {
  parse();
}

MathFunction::~MathFunction() {
  delete f;
}

bool MathFunction::parse() {
  bool r = parser->parse(func_string);

  if(r) {
    // delete old f
    delete f;
    f = parser->getFunction();
  }
  return r;
}

double MathFunction::operator()(const double &d) const {
  return (*f)(d);
}
