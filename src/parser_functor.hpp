#ifndef __PARSER_FUNCTOR_HPP__
#define __PARSER_FUNCTOR_HPP__

#include <cmath>
#include <limits>

/** It was necessary to reimplement subset of the predefined methods and
    classes from the <functional>-header. The reason is simple:
    the supertype template <typename _Res, typename _Arg> std::unary_function
    doesn't provide an operator() to evaluate a function of this type.
    Since unary_function-objects are packed in a stack (for parsing) one needs
    a template type for all these objects, hence this operator is indispensable.

    In these lights all functions and classes operating with unary_functions
    had to be rewritten for this purpose
**/
namespace parser {
  namespace functor {

    // prototypes for unary and binary functions
    class unary_function {
    public:
      unary_function() : __stepsize(0) {}
      virtual double operator()(const double&) const = 0;
      virtual ~unary_function() {}

      // set the stepsize of the display (i.e. the increment in x direction for
      // drawing one line segment of the function graph); used e.g.
      // for computing derivatives and integrals
      virtual void setStepsize(const double &ss) { __stepsize = ss; };

    protected:
      double __r;
      double __stepsize;
    };

    class binary_function { // : public std::binary_function<double, double, double> {
    public:
      virtual double operator()(const double &, const double &) const = 0;
      virtual ~binary_function() { }
    };

    // pointer to unary function type
    class pointer_to_unary_function : public unary_function {
    protected:
      double (*_M_ptr)(double);

    public:
      pointer_to_unary_function() : unary_function() { }

      explicit
      pointer_to_unary_function(double (*__x)(double))
	: _M_ptr(__x) { }

      inline double operator()(const double &__x) const
      { return _M_ptr(__x); }
    };

    /// A pointer_adaptors adaptors for function pointers
    inline pointer_to_unary_function*
    ptr_fun(double (*__x)(double))
    { return new pointer_to_unary_function(__x); }

    // unary arithmetic functors
    class negate : public unary_function
    {
    public:
      inline double operator()(const double &__x) const
      { return -__x; }
    };

    class const_unary_function : public unary_function {
    public:
      const_unary_function(double v) : unary_function() { __r = v; }
      inline double operator()(double const&) const {
	return __r;
      }
    private:
      double __r;
    };

    class identity : public unary_function {
    public:
      inline double operator()(double const &v) const {
	return v;
      }
    };

    class derivative : public unary_function {
    protected:
      unary_function *_M_fn1;

    public:
      derivative(unary_function *__x) : unary_function(), _M_fn1(__x) {}
      ~derivative() {
	delete _M_fn1;
      }

      void setStepsize(const double &ss) {
	__stepsize = ss;
	_M_fn1->setStepsize(ss);
      };

      inline double operator()(const double &d) const
      { return ((*_M_fn1)(d+__stepsize)-(*_M_fn1)(d))/__stepsize; }
    };

    // composition of functions
    class unary_compose : public unary_function
    {
    protected:
      unary_function *_M_fn1;
      unary_function *_M_fn2;

    public:
      unary_compose(unary_function *__x, unary_function *__y)
	: unary_function(), _M_fn1(__x), _M_fn2(__y) {}

      inline ~unary_compose() {
	delete _M_fn1;
	delete _M_fn2;
      }

      void setStepsize(const double &ss) {
	__stepsize = ss;
	_M_fn1->setStepsize(ss);
	_M_fn2->setStepsize(ss);
      };

      inline double operator()(const double& __x) const
      { return (*_M_fn1)((*_M_fn2)(__x)); }
    };

    inline unary_compose*
    compose1(unary_function *__fn1, unary_function *__fn2)
    { return new unary_compose(__fn1, __fn2); }

    class binary_compose : public unary_function
    {
    protected:
      binary_function *_M_fn1;
      unary_function *_M_fn2;
      unary_function *_M_fn3;
      
    public:
      binary_compose(binary_function *__x, unary_function *__y,
		     unary_function *__z)
	: unary_function(), _M_fn1(__x), _M_fn2(__y), _M_fn3(__z) {
      }

      ~binary_compose() {
	delete _M_fn1;
	delete _M_fn2;
	delete _M_fn3;
      }

      void setStepsize(const double &ss) {
	__stepsize = ss;
	_M_fn2->setStepsize(ss);
	_M_fn3->setStepsize(ss);
      };

      inline double operator()(const double& __x) const
      { return (*_M_fn1)((*_M_fn2)(__x), (*_M_fn3)(__x)); }
    };

    inline binary_compose*
    compose2(binary_function* __fn1, unary_function* __fn2,
	     unary_function* __fn3)
    { return new binary_compose(__fn1, __fn2, __fn3); }

    // binary arithmetic functors
    class plus : public binary_function
    {
    public:
      inline double operator()(const double &__x, const double &__y) const
      { return __x + __y; }
    };
    class minus : public binary_function
    {
    public:
      inline double operator()(const double &__x, const double &__y) const
      { return __x - __y; }
    };
    class multiplies : public binary_function
    {
    public:
      inline double operator()(const double &__x, const double &__y) const
      { return __x * __y; }
    };
    class divides : public binary_function
    {
    public:
      inline double operator()(const double &__x, const double &__y) const
      { return __x / __y; }
    };

    class powers : public binary_function
    {
    public:
      inline double operator()(const double& __x, const double& __y) const
      { return std::pow(__x, __y); }
    };
  }

  typedef functor::unary_function un_fun;
}
#endif
