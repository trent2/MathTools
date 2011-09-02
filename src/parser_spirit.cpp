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

/*  file: --- parser_spirit.cpp --- */

#include <boost/spirit/include/qi.hpp>

#include "parser.hpp"
#include "parser_helper.hpp"

namespace parser {
  unary_function_parser unary_function_parser::singleton_parser;

  unary_function_parser* unary_function_parser::getParser() {
    return &singleton_parser;
  }

  unary_function_parser::unary_function_parser() : unary_function_parser::base_type(Start), f(0), func_symbol(new _helper::func_symbol_struct) {
    using qi::double_;
    using qi::lit;
    using qi::char_;
    using qi::string;
    using ascii::no_case;

    using namespace _helper;

    Consts = no_case[string("pi")]                   [_push_const(_eval)] |        // evaluate constants
      no_case[string("e")]                           [_push_const(_eval)];

    Func = (-(*func_symbol) >> '(' >> Start >> ')')     [_do_un_op(_eval)];         // evaluate unary functions (sin, cos, ...)

    P = double_                                      [_push_const(_eval)] |
      no_case[lit('x')]                                [_identity(_eval)] |
      Func | Consts;

    F = P >> -(char_('^') >> P)                       [_do_mul_op(_eval)];         // power-chains wo parentheses are not allowed 

    sF = (char_('-') >> F)                            [_do_un_op(_eval)] |
      -lit('+') >> F;                   // evaluate minus sign in front of a factor

    S = sF                                        >>
      *((+(char_('*') >> sF))                            [_do_mul_op(_eval)] |
	(+(char_('/') >> sF))                            [_do_mul_op(_eval)]);

    Start =  S >> *((+(char_('+') >> S))                  [_do_mul_op(_eval)] |
		    (+(char_('-') >> S))                  [_do_mul_op(_eval)]);
  }

  unary_function_parser::~unary_function_parser() {
    delete func_symbol;
  }

  bool unary_function_parser::parse(const std::string &s) {
    c_str_it i_start = s.begin();
    c_str_it i_end = s.end();

    bool r = phrase_parse(i_start, i_end, *this, ascii::space);
    r = r && (i_start == i_end); 

    if(r) {
      f = _eval.top();
      _eval.pop();
    } else
      // clear stack
      while(!_eval.empty()) {
	delete _eval.top();
	_eval.pop();
      }

    return r;
  }
}
