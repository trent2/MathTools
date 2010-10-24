#ifndef __MATH_FUNCTION_HPP__
#define __MATH_FUNCTION_HPP__

#include <string>
#include <QtGui/QColor>

#include <ginac/ginac.h>

#include "functor_unary.hpp"


class MathFunction {
public:
  MathFunction(const QColor &col = Qt::red);
  MathFunction(const std::string &s, const QColor &col = Qt::red);

  ~MathFunction();

  void setFunction(const std::string &s) {
    mOrig_func_string = s;
  }

  void setColor(const QColor &col) {
    mColor = col;
  }

  const QColor& getColor() const {
    return mColor;
  }

  bool parse();

  double operator()(const double &) const;
  void setUStepsize(const double &d);

private:
  // dirty stuff, don't read on
  static void createReader();
  static GiNaC::parser *reader;

  std::string mOrig_func_string;
  GiNaC::ex mFunc_term;
  functor::un_fun *mNum_Func;
  QColor mColor;
};

#endif
