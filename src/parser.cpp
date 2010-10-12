#include <boost/spirit/include/qi.hpp>

#include "parser.hpp"
#include "parser_helper.hpp"

namespace parser {
  unary_function_parser::unary_function_parser() : unary_function_parser::base_type(Start), f(0), func_symbol(new _helper::func_symbol_struct) {
    using qi::double_;
    using qi::lit;
    using qi::char_;
    using ascii::string;
    using ascii::no_case;

    using namespace _helper;

      Consts = no_case[string("pi")]                   [_push_const(_eval)] |        // evaluate constants
	no_case[string("e")]                           [_push_const(_eval)];

      Func = ((*func_symbol) >> newStart)                   [_do_un_op(_eval)];         // evaluate unary functions (sin, cos, ...)

      newStart = '(' >> Start >> ')';

      P = double_                                      [_push_const(_eval)] |
	no_case[lit('x')]                                [_identity(_eval)] |
	newStart | Func | Consts;

      F = P >> -(char_('^') >> P)                       [_do_bin_op(_eval)];         // power-chains wo parentheses are not allowed 

      S = ((char_('-') >> F)                            [_do_un_op(_eval)] |        // evaluate minus sign in front of a factor
	    -lit('+') >> F )                  >>
	*((char_('*') >> F)                             [_do_bin_op(_eval)] |
	  (char_('/') >> F)                             [_do_bin_op(_eval)]);

      Start =  S >> *((char_('+') >>   S)               [_do_bin_op(_eval)] |
		      (char_('-') >> S)                 [_do_bin_op(_eval)]);
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
