#ifndef __FUNCTOR_HPP__
#define __FUNCTOR_HPP__

#include <cmath>
#include <algorithm>
#include <functional>
#include <list>

#include "functor_unary.hpp"

  /** It was necessary to reimplement subset of the predefined methods and
      classes from the <functional>-header. The reason is simple:
      the supertype template <typename _Res, typename _Arg> std::unary_function
      doesn't provide an operator() to evaluate a function of this type.
      Since unary_function-objects are packed in a stack (for parsing) one needs
      a template type for all these objects, hence this operator is indispensable.

      In these lights all functions and classes operating with unary_functions
      had to be rewritten for this purpose

      The implementation was also optimized for speedy evaluation, i.e. operator()
      is designed to beeing executed as fast as possible.

      R. Spillner, 2010
  **/
namespace functor {

  // prototypes for unary and binary functions
  class multivariate_function {
  public:
    virtual double& operator()(const std::list<double>&) = 0;
    virtual ~multivariate_function() {}
    virtual void setStepsize(const double &ss) { __stepsize = ss; };
  protected:
    double __r;
    double __stepsize;
  };

  // helper class to use a univariate function as a multivariate function
  class uni_as_multivariate_function : public multivariate_function {
  public:
    uni_as_multivariate_function(unary_function *f) : _M_f(f) { }
    ~uni_as_multivariate_function()  { delete _M_f; }

    double& operator()(const std::list<double> &x)
    { return __r = (*_M_f)(x.front()); }

    void setStepsize(const double &ss) {
      __stepsize = ss;
      _M_f->setStepsize(ss);
    };

  protected:
    unary_function *_M_f;
  };

  class const_unary_function : public unary_function {
  public:
    const_unary_function(double v) : _c(v) { }
    inline double& operator()(double const&) {
      return __r = _c;
    }
  protected:
    double _c;
  };

  class identity : public unary_function {
  public:
    inline double& operator()(double const &v) {
      return __r = v;
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

    inline double& operator()(const double &d)
    { return __r = ((*_M_fn1)(d+__stepsize)-(*_M_fn1)(d))/__stepsize; }
  };

  // composition of functions
  class composition : public unary_function
  {
  private:
    struct Delete_ptr {
      template <class T> T*
      operator()(T* x) const { delete x; return 0; }
    };

    struct SetStepsize {
      double _ss;
      SetStepsize(double ss) : _ss(ss) { }
      unary_function* operator()(unary_function* x) const { x->setStepsize(_ss); return x; }
    };

    multivariate_function *_M_mvf;
    std::list<unary_function*> *_M_fs;
    std::list<double> _y;
    std::list<unary_function*>::const_iterator _it;

  public:
    composition(multivariate_function *mvf, std::list<unary_function*> *fs) : _M_mvf(mvf), _M_fs(fs), _y(fs->size()) { }

    inline ~composition() {
      transform(_M_fs->begin(), _M_fs->end(), _M_fs->begin(), Delete_ptr());
      delete _M_fs;
      delete _M_mvf;
    }

    void setStepsize(const double &ss) {
      __stepsize = ss;
      transform(_M_fs->begin(), _M_fs->end(), _M_fs->begin(), SetStepsize(ss));
      _M_mvf->setStepsize(ss);
    };

    inline double& operator()(const double& __x)
    {
      transform(_M_fs->begin(), _M_fs->end(), _y.begin(), std::bind2nd(std::mem_fun(&unary_function::operator()), __x));

      return __r = (*_M_mvf)(_y);
    }
  };

  inline composition*
  compose(multivariate_function* __mvf, std::list<unary_function*> *__fs)
  { return new composition(__mvf, __fs); }

  inline composition*
  compose1(unary_function* __f1, unary_function* __f2) {
    std::list<unary_function*> *v = new std::list<unary_function*>;
    v->push_back(__f2);
    return new composition(new uni_as_multivariate_function(__f1), v);
  }


  // pointer to unary function type
  class pointer_to_unary_function : public unary_function
  {
  protected:
    double (*_M_ptr)(double);

  public:
    pointer_to_unary_function() { }

    explicit
    pointer_to_unary_function(double (*__x)(double))
      : _M_ptr(__x) { }

    double&
    operator()(const double &__x)
    { return __r = _M_ptr(__x); }
  };

  /// A pointer_adaptors adaptors for function pointers
  inline pointer_to_unary_function*
  ptr_fun(double (*__x)(double))
  { return new pointer_to_unary_function(__x); }

  // unary arithmetic functors
  struct negate : public unary_function
  {
    inline double& operator()(const double &__x)
    { return __r = -__x; }
  };

  // binary arithmetic functors
  struct plus : public multivariate_function
  {
    //      std::list<double>::const_iterator _beg, _end;
    struct sum {
      double &_s;
      sum(double &r) : _s(r) { }
      void operator()(const double &d) { _s += d; }
    };

    inline double& operator()(const std::list<double> &x)
    {
      //	__r = 0;
      //	_end = x.end();
      //	for(_beg = x.begin(); _beg != _end; ++_beg) __r += *_beg;
      sum s(__r = 0);
      for_each(x.begin(), x.end(), s);

      return __r;
    }
  };

  struct minus : public multivariate_function
  {
    struct dif {
      double &_d;
      dif(double &r) : _d(r) { }
      void operator()(const double &d) { _d -= d; }
    };
    inline double& operator()(const std::list<double> &x)
    {
      __r = 0;

      if(!x.empty()) {
	dif d(__r = 2*x.back());
	for_each(x.begin(), x.end(), d);
      }
      return __r;
    }
  };

  struct multiplies : public multivariate_function
  {
    std::list<double>::const_iterator _beg, _end;

    inline double& operator()(const std::list<double> &x)
    {
      __r = 1;
      _end = x.end();
      for(_beg = x.begin(); _beg != _end; ++_beg) __r *= *_beg;
      return __r;
    }
  };

  struct divides : public multivariate_function
  {
    struct quo {
      double &_q;
      quo(double &r) : _q(r) { }
      void operator()(const double &d) { _q /= d; }
    };
    inline double& operator()(const std::list<double> &x)
    {
      __r = 1;

      if(!x.empty()) {
	quo q(__r = x.back()*x.back());
	for_each(x.begin(), x.end(), q);
      }
      return __r;
    }
  };

  struct powers : public multivariate_function
  {
    inline double& operator()(const std::list<double> &x)
    {
      
      return __r = std::pow(x.back(), x.front());
    }
  };
}

#endif
