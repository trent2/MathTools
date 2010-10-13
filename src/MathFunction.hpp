#ifndef __MATH_FUNCTION_HPP__
#define __MATH_FUNCTION_HPP__

#include <string>
#include <QColor>

#include "parser_functor.hpp"

namespace parser {
  struct unary_function_parser;
}

class MathFunction {
public:
  MathFunction(const QColor &col = Qt::red);
  MathFunction(const std::string &s, const QColor &col = Qt::red);

  ~MathFunction();

  void setFunction(const std::string &s) {
    func_string = s;
  }

  void setColor(const QColor &col) {
    color = col;
  }

  const QColor& getColor() const {
    return color;
  }

  bool parse();

  double operator()(const double &) const;

  void setUStepsize(const double &d);

private:
  std::string func_string;
  parser::un_fun *f;
  parser::unary_function_parser *parser;
  QColor color;
};

#endif
