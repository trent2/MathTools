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
