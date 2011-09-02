#ifndef __FUNCTOR_HPP__
#define __FUNCTOR_HPP__

#include <cmath>
#include <algorithm>
#include <functional>  // used for bind2nd, mem_fun
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
  template <typename T>
  class multivariate_function {
  public:
    virtual T& operator()(const std::list<T>&) = 0;
    virtual ~multivariate_function() {}
    virtual void setStepsize(const double &ss) { __stepsize = ss; };
  protected:
    T __r;
    double __stepsize;
  };

  // helper class to use a univariate function as a multivariate function
  template <typename T>
  class uni_as_multivariate_function : public multivariate_function<T> {
  public:
    uni_as_multivariate_function(typename un_fun<T>::type *f) : _M_f(f) { }
    ~uni_as_multivariate_function()  { delete _M_f; }

    T& operator()(const std::list<T> &x)
    { return this->__r = (*_M_f)(x.front()); }

    void setStepsize(const double &ss) {
      this->__stepsize = ss;
      _M_f->setStepsize(ss);
    };

  protected:
    unary_function<T> *_M_f;
  };

  template <typename T>
  class const_unary_function : public un_fun<T>::type {
  public:
    const_unary_function(T v) : _c(v) { }
    inline T& operator()(T const&) {
      return this->__r = _c;
    }
  protected:
    T _c;
  };

  template <typename T>
  class identity : public un_fun<T>::type {
  public:
    inline T& operator()(T const &v) {
      return this->__r = v;
    }
  };

  template <typename T>
  class derivative : public un_fun<T>::type {
  protected:
    typename un_fun<T>::type *_M_fn1;

  public:
    derivative(typename un_fun<T>::type *__x) : un_fun<T>::type(), _M_fn1(__x) {}
    ~derivative() {
      delete _M_fn1;
    }

    void setStepsize(const double &ss) {
      this->__stepsize = ss;
      _M_fn1->setStepsize(ss);
    };

    inline T& operator()(const T &d)
    { return this->__r = ((*_M_fn1)(d+this->__stepsize)-(*_M_fn1)(d))/this->__stepsize; }
  };

  // composition of functions
  template <typename T>
  class composition : public un_fun<T>::type
  {
  private:
    struct Delete_ptr {
      template <class Cl> Cl*
      operator()(Cl* x) const { delete x; return 0; }
    };

    struct SetStepsize {
      double _ss;
      SetStepsize(double ss) : _ss(ss) { }
      typename un_fun<T>::type* operator()(typename un_fun<T>::type* x) const { x->setStepsize(_ss); return x; }
    };

    multivariate_function<T> *_M_mvf;
    std::list<typename un_fun<T>::type*> *_M_fs;
    std::list<T> _y;
    typename std::list<typename un_fun<T>::type*>::const_iterator _it;

  public:
    composition(multivariate_function<T> *mvf, std::list<typename un_fun<T>::type*> *fs) : _M_mvf(mvf), _M_fs(fs), _y(fs->size()) { }

    inline ~composition() {
      transform(_M_fs->begin(), _M_fs->end(), _M_fs->begin(), Delete_ptr());
      delete _M_fs;
      delete _M_mvf;
    }

    void setStepsize(const double &ss) {
      this->__stepsize = ss;
      transform(_M_fs->begin(), _M_fs->end(), _M_fs->begin(), SetStepsize(ss));
      _M_mvf->setStepsize(ss);
    };

    inline T& operator()(const T& __x)
    {
      transform(_M_fs->begin(), _M_fs->end(), _y.begin(), std::bind2nd(std::mem_fun(&un_fun<T>::type::operator()), __x));

      return this->__r = (*_M_mvf)(_y);
    }
  };

  template <typename T>
  inline composition<T>*
  compose(multivariate_function<T>* __mvf, std::list<typename un_fun<T>::type*> *__fs)
  { return new composition<T>(__mvf, __fs); }

  template <typename T>
  inline composition<T>*
  compose1(typename un_fun<T>::type* __f1, typename un_fun<T>::type* __f2) {
    std::list<typename un_fun<T>::type*> *v = new std::list<typename un_fun<T>::type*>;
    v->push_back(__f2);
    return new composition<T>(new uni_as_multivariate_function<T>(__f1), v);
  }


  // pointer to unary function type
  template <typename T>
  class pointer_to_unary_function : public un_fun<T>::type
  {
  protected:
    T (*_M_ptr_const)(const T&);
    T (*_M_ptr)(T);

  public:
    pointer_to_unary_function() { }

    explicit
    pointer_to_unary_function(T (*__x)(const T&))
    : _M_ptr_const(__x), _M_ptr(0) { }
    pointer_to_unary_function(T (*__x)(T))
    : _M_ptr_const(0), _M_ptr(__x) { }

    T&
    operator()(const T &__x)
    { return this->__r = (_M_ptr == 0 ? _M_ptr_const(__x) : _M_ptr(__x)); }

    // T&
    // operator()(T __x)
    // { return this->__r = (_M_ptr_const == 0 ? M_ptr(__x) : _M_ptr_const(__x)); }
  };

  /// A pointer_adaptors adaptors for function pointers
  template <typename T>
  inline pointer_to_unary_function<T>*
  ptr_fun(T (*__x)(const T&))
  { return new pointer_to_unary_function<T>(__x); }

  template <typename T>
  inline pointer_to_unary_function<T>*
  ptr_fun(T (*__x)(T))
  { return new pointer_to_unary_function<T>(__x); }

  // unary arithmetic functors
  template <typename T>
  struct negate : public un_fun<T>::type
  {
    inline T& operator()(const T &__x)
    { return this->__r = -__x; }
  };

  // binary arithmetic functors
  template <typename T>
  struct plus : public multivariate_function<T>
  {
    //      std::list<double>::const_iterator _beg, _end;
    struct sum {
      T &_s;
      sum(T &r) : _s(r) { }
      void operator()(const T &d) { _s += d; }
    };

    inline T& operator()(const std::list<T> &x)
    {
      //	__r = 0;
      //	_end = x.end();
      //	for(_beg = x.begin(); _beg != _end; ++_beg) __r += *_beg;
      sum s(this->__r = 0);
      for_each(x.begin(), x.end(), s);

      return this->__r;
    }
  };

  template <typename T>
  struct minus : public multivariate_function<T>
  {
    struct dif {
      T &_d;
      dif(T &r) : _d(r) { }
      void operator()(const T &d) { _d -= d; }
    };
    inline T& operator()(const std::list<T> &x)
    {
      this->__r = 0;

      if(!x.empty()) {
	dif d(this->__r = 2*x.back());
	for_each(x.begin(), x.end(), d);
      }
      return this->__r;
    }
  };

  template <typename T>
  struct multiplies : public multivariate_function<T>
  {
    typename std::list<T>::const_iterator _beg, _end;

    inline T& operator()(const std::list<T> &x)
    {
      this->__r = 1;
      _end = x.end();
      for(_beg = x.begin(); _beg != _end; ++_beg) this->__r *= *_beg;
      return this->__r;
    }
  };

  template <typename T>
  struct divides : public multivariate_function<T>
  {
    struct quo {
      T &_q;
      quo(T &r) : _q(r) { }
      void operator()(const T &d) { _q /= d; }
    };
    inline T& operator()(const std::list<T> &x)
    {
      this->__r = 1;

      if(!x.empty()) {
	quo q(this->__r = x.back()*x.back());
	for_each(x.begin(), x.end(), q);
      }
      return this->__r;
    }
  };

  template <typename T>
  struct powers : public multivariate_function<T>
  {
    inline T& operator()(const std::list<T> &x)
    {
      
      return this->__r = std::pow(x.back(), x.front());
    }
  };
}

#endif
