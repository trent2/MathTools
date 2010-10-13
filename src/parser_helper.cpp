#include "parser_helper.hpp"

namespace parser {
  namespace _helper {
    using namespace parser::functor;

    func_symbol_struct::func_symbol_struct() {
      add("sin", sin)
	("cos", cos)
	("tan", tan)
	("atan", atan)
	("exp", exp)
	("ln", ln)
	("lg", lg)
	("sqrt", sqrt)
	("abs", abs)
	("D", D);
    }

    void _push_const::operator()(const double &d, qi::unused_type, bool&) const {
      _eval.push(new const_unary_function(d));
    }

    void _push_const::operator()(const std::string &s, qi::unused_type, bool&) const {
      if(s == ("pi"))
	_eval.push(new const_unary_function(pi));
      else
	if(s == ("e"))
	  _eval.push(new const_unary_function(exp1));
    }

    void _identity::operator()(qi::unused_type, qi::unused_type, bool&) const {
      _eval.push(new identity());
    }

    void _do_bin_op::operator()(unsigned int op_code, qi::unused_type, bool&) const {
      unary_function *rhs = _eval.top();
      _eval.pop();
      unary_function *lhs = _eval.top();
      _eval.pop();

      switch(op_code) {
	// implicit type cast to unsigned int
      case '+': _eval.push(compose2(new plus(), lhs, rhs));break;
      case '-': _eval.push(compose2(new minus(), lhs, rhs));break;
      case '*': _eval.push(compose2(new multiplies(), lhs, rhs));break;
      case '/': _eval.push(compose2(new divides(), lhs, rhs));break;
      case '^': _eval.push(compose2(new powers(), lhs, rhs));break;
      default: _eval.push(lhs); _eval.push(rhs); break;
      }
    }

    void _do_un_op::operator()(unsigned int op_code, qi::unused_type, bool&) const {
      unary_function *f = _eval.top();
      _eval.pop();

      switch(op_code) {
	// implicit type cast to unsigned int
      case '-': _eval.push(compose1(new negate(), f));break;
      case sin: _eval.push(compose1(ptr_fun(std::sin), f));break;
      case cos: _eval.push(compose1(ptr_fun(std::cos), f));break;
      case tan: _eval.push(compose1(ptr_fun(std::tan), f));break;
      case atan:_eval.push(compose1(ptr_fun(std::atan), f));break;
      case exp: _eval.push(compose1(ptr_fun(std::exp), f));break;
      case ln: _eval.push(compose1(ptr_fun(std::log), f));break;
      case lg: _eval.push(compose1(compose2(new divides(), ptr_fun(std::log), new const_unary_function(std::log(10))), f)); break;
      case sqrt: _eval.push(compose1(ptr_fun(std::sqrt), f));break;
      case abs: _eval.push(compose1(ptr_fun(std::abs), f));break;
      case D: _eval.push(new derivative(f));break;
      default: _eval.push(f);
      }
    }
  }
}
