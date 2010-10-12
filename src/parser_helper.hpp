#ifndef __PARSER_HELPER_HPP__
#define __PARSER_HELPER_HPP__
#include <stack>

#include <boost/spirit/include/qi.hpp>

#include "parser_functor.hpp"

namespace parser {
  typedef std::stack<un_fun*> ustack;

  namespace _helper {
    const double pi = std::atan(1)*4;
    const double exp1 = std::exp(1);
    enum parser_fun { sin, cos, tan, atan, exp, ln, lg, sqrt, abs };

    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

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

    struct _do_bin_op {
      _do_bin_op(ustack &eval) : _eval(eval) { }
      void operator()(unsigned int op_code, qi::unused_type, bool&) const;
      ustack &_eval;
    };

    struct _do_un_op {
      _do_un_op(ustack &eval) : _eval(eval) { }
      void operator()(unsigned int op_code, qi::unused_type, bool&) const;
      ustack &_eval;
    };
  }
}
#endif
