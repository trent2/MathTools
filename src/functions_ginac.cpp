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

/*  file: --- functions_ginac.cpp --- */

#include "functions_ginac.hpp"

using namespace GiNaC;

GiNaC::symbol symb_x("x");
GiNaC::symbol symb_e("e");
GiNaC::symbol symb_z("z");
GiNaC::symbol symb_i("i");

static ex sqrt_eval(const ex &x) {
  return sqrt(x);
}

static ex sqrt_evalf(const ex &x) {
  if(is_exactly_a<numeric>(x))
    return sqrt(ex_to<numeric>(x));
  else
    return log(x).hold();
}

static ex lg_eval(const ex &x) {
  return log(x)/log(10);
}

static ex lg_evalf(const ex &x) {
  if(is_exactly_a<numeric>(x))
    return log(ex_to<numeric>(x))/log(10.0);
  else
    return log(x).hold()/log(10.0);
}

static ex lg_deriv(const ex &x, unsigned diff_param) {
  return pow(log(10)*log(x),-1);
}


static ex ln_eval(const ex &x) {
  return log(x);
}

static ex ln_evalf(const ex &x) {
  if(is_a<numeric>(x))
    return log(ex_to<numeric>(x));
  else
    return log(x).hold();
}

static ex ln_deriv(const ex &x, unsigned diff_param) {
  return pow(log(x),-1);
}

static ex D_eval(const ex &x) {
  return x.diff(symbol(symb_x));
}

static ex D_evalf(const ex &x) {
  if(is_a<numeric>(x))
    return 0;
  else
    return x.diff(symbol("x"));
}

REGISTER_FUNCTION(sqrt, eval_func(sqrt_eval).
		  evalf_func(sqrt_evalf).
		  latex_name("\\sqrt"));

REGISTER_FUNCTION(lg, eval_func(lg_eval).
		  evalf_func(lg_evalf).
		  derivative_func(lg_deriv).
		  latex_name("\\lg"));

REGISTER_FUNCTION(ln, eval_func(ln_eval).
		  evalf_func(ln_evalf).
		  derivative_func(ln_deriv).
		  latex_name("\\ln"));

REGISTER_FUNCTION(D, eval_func(D_eval).
		  evalf_func(D_evalf).
		  latex_name("D"));
