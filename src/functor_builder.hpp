#ifndef __FUNCTOR_BUILDER_HPP__
#define __FUNCTOR_BUILDER_HPP__

#include <ginac/ginac.h>
#include "functor_unary.hpp"


/** Recursively build function trees from GiNaC-expressions for
 *  fast numerical evaluation of functions.
 */
namespace functor {
  template <typename T>
  struct numeric_function_builder {
    enum _func_enum { sin, cos, tan, atan, exp, log, ln, lg, sqrt, abs, D };

    static typename un_fun<T>::type* create_func(const GiNaC::ex &);
    static std::list<typename un_fun<T>::type*>* create_func_list(const GiNaC::ex &);
  };
}

#include "functor_builder.cpp"
#endif
