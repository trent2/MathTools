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

/*  file: --- functor_unary.hpp --- */

#ifndef __FUNCTOR_UNARY_HPP__
#define __FUNCTOR_UNARY_HPP__
namespace functor {
  template <typename T>
  class unary_function {
  public:
    unary_function() : __stepsize(0) {}
    virtual T& operator()(const T &d) = 0;
    virtual ~unary_function() {}

    // set the stepsize for infinitesemal operations, e.g.
    // for computing derivatives and integrals
    virtual void setStepsize(const double &ss) { __stepsize = ss; };

  protected:
    T __r;
    double __stepsize;
  };

  template <typename T>
  struct un_fun {
    typedef unary_function<T> type;
  };
}
#endif
