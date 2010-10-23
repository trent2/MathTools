#ifndef __FUNCTOR_BUILDER_HPP__
#define __FUNCTOR_BUILDER_HPP__

#include <ginac/ginac.h>
#include "functor_unary.hpp"


/** Recursively build function trees from GiNaC-expressions for
 *  fast numerical evaluation of functions.
 */
namespace functor {
  struct numeric_function_builder {
    enum _func_enum { sin, cos, tan, atan, exp, log, ln, lg, sqrt, abs, D };

    static un_fun* create_func(const GiNaC::ex &);
    static std::list<un_fun*>* create_func_list(const GiNaC::ex &);
  };
}
#endif
