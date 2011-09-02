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

/*  file: --- parser_spirit_helper.cpp --- */

#include <list>
#include "parser_helper.hpp"
#include "functor.hpp"

namespace parser {
  namespace _helper {
    const double pi = std::atan(1)*4;
    const double exp1 = std::exp(1);

    using namespace functor;

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

    void _do_mul_op::operator()(const std::vector<char>& c, qi::unused_type, bool&) const {
      if(c.size()) {
	std::list<un_fun*> *v = new std::list<un_fun*>;
	
	for(int i=0; i<=c.size(); ++i) {
	  v->push_back(_eval.top());
	  _eval.pop();
	}

	switch(c[0]) {
	  // implicit type cast to unsigned int
	case '+': _eval.push(compose(new plus(), v));break;
	case '-': _eval.push(compose(new minus(), v));break;
	case '*': _eval.push(compose(new multiplies(), v));break;
	case '/': _eval.push(compose(new divides(), v));break;
	// put elements back
	default: for(std::list<un_fun*>::reverse_iterator it = v->rbegin(); it != v->rend(); ++it)
	    _eval.push(*it);
	}
      }
    }

    void _do_mul_op::operator()(const char& c, qi::unused_type, bool&) const {
      if(c == '^') {
	std::list<un_fun*> *v = new std::list<un_fun*>;
	v->push_back(_eval.top());
	_eval.pop();
	v->push_back(_eval.top());
	_eval.pop();

	_eval.push(compose(new powers(), v));
      }
    }

    void _do_un_op::operator()(optional<unsigned int> op_code, qi::unused_type, bool&) const {
      if(op_code) {
	std::list<un_fun*> *v;
	un_fun *f = _eval.top();
	_eval.pop();

	switch(*op_code) {
	  // implicit type cast to unsigned int
	case '-': _eval.push(compose1(new negate(), f));break;
	case sin: _eval.push(compose1(ptr_fun(std::sin), f));break;
	case cos: _eval.push(compose1(ptr_fun(std::cos), f));break;
	case tan: _eval.push(compose1(ptr_fun(std::tan), f));break;
	case atan:_eval.push(compose1(ptr_fun(std::atan), f));break;
	case exp: _eval.push(compose1(ptr_fun(std::exp), f));break;
	case ln: _eval.push(compose1(ptr_fun(std::log), f));break;
	case lg: v = new std::list<un_fun*>;
	  v->push_back(compose1(ptr_fun(std::log), f));
	  v->push_back(new const_unary_function(std::log(10)));
	  _eval.push(compose(new divides(), v)); break;
	case sqrt: _eval.push(compose1(ptr_fun(std::sqrt), f));break;
	case abs: _eval.push(compose1(ptr_fun(std::abs), f));break;
	case D: _eval.push(new derivative(f));break;
	default: _eval.push(f);
	}
      }
    }
  }
}
