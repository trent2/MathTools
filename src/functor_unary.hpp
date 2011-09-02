#ifndef __FUNCTOR_UNARY_HPP__
#define __FUNCTOR_UNARY_HPP__
namespace functor {
  template <typename T>
  class unary_function {
  public:
    unary_function() : __stepsize(0) {}
    virtual T& operator()(const T &d) = 0;
    virtual ~unary_function() {}

    // set the stepsize for infinitesemal operations, e.g.
    // for computing derivatives and integrals
    virtual void setStepsize(const double &ss) { __stepsize = ss; };

  protected:
    T __r;
    double __stepsize;
  };

  template <typename T>
  struct un_fun {
    typedef unary_function<T> type;
  };
}
#endif
