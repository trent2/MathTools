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

  class unary_function_parser : public qi::grammar<c_str_it, ascii::space_type> {
  public:
    static unary_function_parser* getParser();

    bool parse(const std::string &s);
    inline un_fun* getFunction() const {
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
    un_fun *f;  // return function object
    _helper::func_symbol_struct *func_symbol;

    unary_function_parser();
    ~unary_function_parser();


    static unary_function_parser singleton_parser;
  };
}
#endif
