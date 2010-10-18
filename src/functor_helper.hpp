#ifndef __FUNCTOR_HELPER_HPP__
#define __FUNCTOR_HELPER_HPP__

#include "functor.hpp"

namespace functor {
  namespace helper {

    bool isPolynomial(functor::un_fun *f) {
      using namespace functor;

      binary_function *bf;
      unary_compose *uc;
      binary_compose *bc;

      if(dynamic_cast<pointer_to_unary_function>(f))
	return false;

      if(uc = dynamic_cast<unary_compose>(f))
	return isPolynomial(uc->getF1()) && isPolynomial(uc->getF2());

      if(bc = dynamic_cast<binary_compose>(f)) {
	if(dynamic_cast<divides>(bc->getF1()))
	  return !hasX(bc->getF3());
	else if(dynamic_cast<powers>(bc->getF1()))
	  return dynamic_cast<const_unary_function>(bc->getF3());
	else
	  return isPolynomial(bc->getF2()) && isPolynomial(bc->getF3());
      }

      return true;
    }

    bool hasX(functor::un_fun *f) {
      using namespace functor;

      if(dynamic_cast<identity>(f))
	return true;

      if(uc = dynamic_cast<unary_compose>(f))
	return hasX(uc->getF1()) || hasX(uc->getF2());

      if(bc = dynamic_cast<binary_compose>(f))
	return hasX(bc->getF2()) || hasX(bc->getF3());

      return false;
    }
  }
}
#endif
