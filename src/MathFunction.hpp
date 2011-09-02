/*  MathTools 
    Copyright (C) 2010, 2011, 2012 Robert Spillner <Robert.Spillner@gmx.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*  file: --- MathFunction.hpp --- */

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
