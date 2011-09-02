#ifndef __MATH_FUNCTION_HPP__
#define __MATH_FUNCTION_HPP__

#include <string>

#include <QtGui/QColor>
#include <ginac/ginac.h>

#include "functor_unary.hpp"

// the argument type of the function (e.g. double or complex)
template <typename T>
class MathFunction {
public:
  MathFunction(const std::string &s="", const QColor &col = Qt::red);

  ~MathFunction();

  void setFunction(const std::string&);

  void setColor(const QColor &col) { mColor = col; }
  const QColor& getColor() const { return mColor; }

  bool parseOk() const { return mParseOk; }

  T operator()(const T &) const;
  void setUStepsize(const double &d);

private:
  // dirty stuff, don't read on
  static void createReader();
  static GiNaC::parser *reader;

  void parse();

  std::string mOrig_func_string;
  GiNaC::ex mFunc_term;
  typename functor::un_fun<T>::type *mNum_Func;
  QColor mColor;
  bool mParseOk;
};

#include "MathFunction.cpp"

#endif
