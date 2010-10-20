#ifndef __FUNCTOR_UNARY_HPP__
#define __FUNCTOR_UNARY_HPP__
namespace functor {
  class unary_function {
  public:
    unary_function() : __stepsize(0) {}
    virtual double& operator()(const double &d) = 0;
    virtual ~unary_function() {}

    // set the stepsize for infinitesemal operations, e.g.
    // for computing derivatives and integrals
    virtual void setStepsize(const double &ss) { __stepsize = ss; };

  protected:
    double __r;
    double __stepsize;
  };

  typedef unary_function un_fun;
}
#endif
