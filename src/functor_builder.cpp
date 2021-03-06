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

/*  file: --- functor_builder.cpp --- */

// #include "functor_builder.hpp"
#include <complex>
#include <exception>
#include "functor.hpp"

namespace std {
  const std::complex<double> M_I = std::complex<double>(0,1);

  inline std::complex<double> asin(const std::complex<double> &c) {
    return -M_I*log(M_I*c+sqrt(1.0-c*c));
  }

  inline std::complex<double> acos(const std::complex<double> &c) {
    return -M_I*log(c+M_I*sqrt(1.0-c*c));
  }

  inline std::complex<double> atan(const std::complex<double> &c) {
    return log((1.0+M_I*c)/(1.0-M_I*c))/(2.0*M_I);
  }
  inline std::complex<double> abs(const std::complex<double> &c) {
    // reference double std::abs(const std::complex<double> &) by "abs_double"
    // to force the compiler using the correct method since
    // using "std::abs(c)" in the return statement calls this function recursively.
    double(*abs_double)(const std::complex<double> &) = std::abs;
    return abs_double(c);
  }
}

namespace functor {
  // extern const_unary_function<double>*         ___produceI(const double& );
  // extern const_unary_function<std::complex<double> >* ___produceI(const std::complex<double> &);

  inline const_unary_function<double>*
  ___produceI(const double& ) {
    throw std::bad_typeid();
  }

  inline const_unary_function<std::complex<double> >*
  ___produceI(const std::complex<double> &) {
    return new const_unary_function<std::complex<double> >(std::M_I);
  }


  template <class T>
  typename un_fun<T>::type* numeric_function_builder<T>::create_func(const GiNaC::ex &e) {
    using GiNaC::ex;
    using GiNaC::is_a;
    using GiNaC::ex_to;
    using GiNaC::add;
    using GiNaC::mul;
    using GiNaC::power;
    using GiNaC::numeric;
    using GiNaC::constant;
    using GiNaC::symbol;
    using GiNaC::symtab;
    using GiNaC::function;

    std::list<typename un_fun<T>::type*> *v;

    if(is_a<add>(e))
      return compose(new plus<T>(), create_func_list(e));
    else if(is_a<mul>(e))
      return compose(new multiplies<T>(), create_func_list(e));
    else if(is_a<power>(e))
      return compose(new powers<T>(), create_func_list(e));
    else if(is_a<function>(e)) {
      std::string fn = ex_to<function>(e).get_name();
      if(fn=="sin")
	return compose1<T>(ptr_fun<T>(std::sin), create_func(e.op(0)));
      else if(fn=="cos")
	return compose1<T>(ptr_fun<T>(std::cos), create_func(e.op(0)));
      else if(fn=="tan")
       	return compose1<T>(ptr_fun<T>(std::tan), create_func(e.op(0)));
      else if(fn=="asin")
	return compose1<T>(ptr_fun<T>(std::asin), create_func(e.op(0)));
      else if(fn=="acos")
	return compose1<T>(ptr_fun<T>(std::acos), create_func(e.op(0)));
      else if(fn=="atan")
	return compose1<T>(ptr_fun<T>(std::atan), create_func(e.op(0)));
      else if(fn=="exp")
	return compose1<T>(ptr_fun<T>(std::exp), create_func(e.op(0)));
      else if(fn=="log")
	return compose1<T>(ptr_fun<T>(std::log), create_func(e.op(0)));
      else if(fn=="ln")
	return compose1<T>(ptr_fun<T>(std::log), create_func(e.op(0)));
      else if(fn=="lg") {
	v = new std::list<typename un_fun<T>::type*>;
	v->push_back(compose1<T>(ptr_fun<T>(std::log), create_func(e.op(0))));
	v->push_back(new const_unary_function<T>(std::log(10)));
	return compose<T>(new divides<T>(), v);
      }
      else if(fn=="abs")
	return compose1<T>(ptr_fun<T>(std::abs), create_func(e.op(0)));
      else if(fn=="sqrt")
	return compose1<T>(ptr_fun<T>(std::sqrt), create_func(e.op(0)));
      else if(fn=="D")
	return new derivative<T>(create_func(e.op(0)));
    }
    else if(is_a<numeric>(e) || is_a<constant>(e))
      return new const_unary_function<T>(ex_to<numeric>(e.evalf()).to_double());
    else if(is_a<symbol>(e)) {
      if(ex_to<symbol>(e).get_name()=="e")
	return new const_unary_function<T>(std::exp(1));
      else if(ex_to<symbol>(e).get_name()=="i") {
	return ___produceI(T());
      }
      else
	return new identity<T>();
    }

    return 0;
  }

  template <typename T>
  std::list<typename un_fun<T>::type*>* numeric_function_builder<T>::create_func_list(const GiNaC::ex &e) {
    std::list<typename un_fun<T>::type*> *v = new std::list<typename un_fun<T>::type*>;

    for(GiNaC::const_iterator it = e.begin(); it != e.end(); ++it)
      v->push_front(create_func(*it));

    return v;
  }
}
