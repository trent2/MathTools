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

/*  file: --- parser_spirit_helper.hpp --- */

#ifndef __PARSER_HELPER_HPP__
#define __PARSER_HELPER_HPP__
#include <stack>
#include <vector>

#include <boost/spirit/include/qi_symbols.hpp>

#include "functor_unary.hpp"

namespace parser {
  typedef std::stack<functor::unary_function*> ustack;

  namespace _helper {
    enum parser_fun { sin, cos, tan, atan, exp, ln, lg, sqrt, abs, D };

    namespace qi = boost::spirit::qi;

    using boost::optional;

    struct func_symbol_struct : qi::symbols<char, unsigned> {
      func_symbol_struct();
    };
  
    struct _push_const {
      _push_const(ustack &eval) : _eval(eval) { }
      void operator()(const double&, qi::unused_type, bool&) const;
      void operator()(const std::string&, qi::unused_type, bool&) const;
      ustack &_eval;
    };

    struct _identity {
      _identity(ustack &eval) : _eval(eval) { }
      void operator()(qi::unused_type, qi::unused_type, bool&) const;
      ustack &_eval;
    };

    struct _do_mul_op {
      _do_mul_op(ustack &eval) : _eval(eval) { }
      void operator()(const std::vector<char>&, qi::unused_type, bool&) const;
      void operator()(const char&, qi::unused_type, bool&) const;
      ustack &_eval;
    };

    struct _do_un_op {
      _do_un_op(ustack &eval) : _eval(eval) { }
      void operator()(optional<unsigned int> op_code, qi::unused_type, bool&) const;
      ustack &_eval;
    };
  }
}
#endif
