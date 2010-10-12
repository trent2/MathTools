#ifndef __PARSER_HPP__
#define __PARSER_HPP__
#include <string>
#include <stack>

#include <boost/spirit/include/qi.hpp>

#include "parser_functor.hpp"

namespace parser {

  namespace _helper {
    struct func_symbol_struct;
  }

  typedef std::stack<un_fun*> ustack;

  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  typedef std::string::const_iterator c_str_it;

  struct unary_function_parser : qi::grammar<c_str_it, ascii::space_type> {
    // using functors to generate an evaluable function from the string
    qi::rule<c_str_it, ascii::space_type> Start, S, F, P, newStart, Func, Consts;
    ustack _eval;
    un_fun *f;  // return function object
    _helper::func_symbol_struct *func_symbol;

    unary_function_parser();
    ~unary_function_parser();

    bool parse(const std::string &s);

    inline un_fun* getFunction() const {
      return f;
    }
  };
}
#endif
