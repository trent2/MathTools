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

/*  file: --- parser_spirit.hpp --- */

#ifndef __PARSER_HPP__
#define __PARSER_HPP__
#include <string>
#include <stack>

#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_char_class.hpp>

#include "functor_unary.hpp"

namespace parser {

  namespace _helper {
    struct func_symbol_struct;
  }

  typedef std::stack<functor::un_fun*> ustack;

  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  typedef std::string::const_iterator c_str_it;

  class unary_function_parser : public qi::grammar<c_str_it, ascii::space_type> {
  public:
    static unary_function_parser* getParser();

    bool parse(const std::string &s);
    inline functor::un_fun* getFunction() const {
      return f;
    }
  protected:
    // using functors to generate an evaluable function from the string
    qi::rule<c_str_it, ascii::space_type> Start,                       // start non-terminal
      S,                                                               // summand non-terminal
      F,                                                               // factor non-terminal
      sF,                                                              // signed factor non-terminal
      P,                                                               // exponential (power) non-terminal
      Func,                                                            // (unary) function non-terminal
      Consts;                                                          // constant non-terminal
    ustack _eval;
    functor::un_fun *f;  // return function object
    _helper::func_symbol_struct *func_symbol;

    unary_function_parser();
    ~unary_function_parser();


    static unary_function_parser singleton_parser;
  };
}
#endif
