#include "functor_builder.hpp"
#include "functor.hpp"

namespace functor {
  un_fun* numeric_function_builder::create_func(const GiNaC::ex &e) {
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

    std::list<un_fun*> *v;

    if(is_a<add>(e))
      return compose(new plus(), create_func_list(e));
    else if(is_a<mul>(e))
      return compose(new multiplies(), create_func_list(e));
    else if(is_a<power>(e))
      return compose(new powers(), create_func_list(e));
    else if(is_a<function>(e)) {
      std::string fn = ex_to<function>(e).get_name();
      if(fn=="sin")
	return compose1(ptr_fun(std::sin), create_func(e.op(0)));
      else if(fn=="cos")
	return compose1(ptr_fun(std::cos), create_func(e.op(0)));
      else if(fn=="tan")
	return compose1(ptr_fun(std::tan), create_func(e.op(0)));
      else if(fn=="atan")
	return compose1(ptr_fun(std::atan), create_func(e.op(0)));
      else if(fn=="exp")
	return compose1(ptr_fun(std::exp), create_func(e.op(0)));
      else if(fn=="log")
	return compose1(ptr_fun(std::log), create_func(e.op(0)));
      else if(fn=="ln")
	return compose1(ptr_fun(std::log), create_func(e.op(0)));
      else if(fn=="lg") {
	v = new std::list<un_fun*>;
	v->push_back(compose1(ptr_fun(std::log), create_func(e.op(0))));
	v->push_back(new const_unary_function(std::log(10)));
	return compose(new divides(), v);
      }
      else if(fn=="sqrt")
	return compose1(ptr_fun(std::sqrt), create_func(e.op(0)));
      else if(fn=="abs")
	return compose1(ptr_fun(std::abs), create_func(e.op(0)));
      else if(fn=="D")
	return new derivative(create_func(e.op(0)));
    }
    else if(is_a<numeric>(e) || is_a<constant>(e))
      return new const_unary_function(ex_to<numeric>(e.evalf()).to_double());
    else if(is_a<symbol>(e)) {
      if(ex_to<symbol>(e).get_name()=="e")
	return new const_unary_function(std::exp(1));
      else
	return new identity();
    }

    return 0;
  }

  std::list<un_fun*>* numeric_function_builder::create_func_list(const GiNaC::ex &e) {
    std::list<un_fun*> *v = new std::list<un_fun*>;

    for(GiNaC::const_iterator it = e.begin(); it != e.end(); ++it)
      v->push_front(create_func(*it));

    return v;
  }
}
