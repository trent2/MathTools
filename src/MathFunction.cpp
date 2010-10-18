#include "MathFunction.hpp"
#include "parser.hpp"

using parser::unary_function_parser;

MathFunction::MathFunction(const QColor &col) : func_string(""), mF(0), parser(unary_function_parser::getParser()), color(col) { }

MathFunction::MathFunction(const std::string &s, const QColor &col) : func_string(s), mF(0),
								      parser(unary_function_parser::getParser()), color(col) {
  parse();
}

MathFunction::~MathFunction() {
  delete mF;
}

bool MathFunction::parse() {
  bool r = parser->parse(func_string);

  if(r) {
    // delete old mF
    delete mF;
    mF = parser->getFunction();
  }
  return r;
}

void MathFunction::setUStepsize(const double &d) {
  if(mF)
    mF->setStepsize(d);
}

double MathFunction::operator()(const double &d) const {
  return (*mF)(d);
}


