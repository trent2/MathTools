#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

#include <iostream>
#include <string>
#include <cmath>

#include "parser_functions.hpp"

namespace parser {
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  namespace phoenix = boost::phoenix;

  typedef std::string::const_iterator __cs_it;

  struct arith_expr_grammar : qi::grammar<__cs_it, double(), ascii::space_type> {

    // That something like this is possible in a language...
    arith_expr_grammar() : arith_expr_grammar::base_type(Start), x(new double(0)) {
      using qi::double_;
      using phoenix::ref;
      using qi::_val;
      using qi::_1;
      using qi::_2;
      using qi::lit;
      using qi::eps;
      using ascii::no_case;

      Consts = no_case["pi"][_val = pi] | no_case['e'] [_val = exp1];

      newStart = '(' >> Start [_val = _1] >> ')';

      Func = (func_symbol >> newStart) [_val = unary_function_lazy(_1, _2)];

      P = double_[_val = _1] | no_case[lit('x')] [_val = ref(*x)] |
	  newStart [_val = _1] | Func [ _val = _1] | Consts [_val = _1];

      F = P [_val = _1] >> -('^' >> P [pot_lazy(_val, _1)]);

          // evaluate possible minus-sign
      S = eps[_val = 1] >> (-(lit('-')[_val = -1]) >> F ) [_val *= _1]
                         >> *('*' >> F [_val *= _1] | '/' >> F [ _val = _1]);
      
      Start =  S [_val = _1] >> *('+' >> S [_val += _1]
				  | '-' >> S [_val -= _1]);
    }

    double eval(const double &d, const std::string &s) const {
      double result;
      __cs_it i_start = s.begin();
      __cs_it i_end = s.end();

      *x = d;
      phrase_parse(i_start, i_end, *this, ascii::space, result);

      return result;
    }

    bool valid(const std::string &s) const {
      __cs_it i_start = s.begin();
      __cs_it i_end = s.end();

      bool r = phrase_parse(i_start, i_end, *this, ascii::space);
      return r && (i_start == i_end);
    }

    qi::rule<__cs_it, double(), ascii::space_type> Start, S, F, P, Func, newStart, Consts;
    double *x;
  };

  static const arith_expr_grammar arith_parser;
}
#endif
