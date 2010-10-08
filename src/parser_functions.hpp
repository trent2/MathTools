#ifndef __FUNCTIONS_HPP__
#define __FUNCTIONS_HPP__

#include <cmath>
#include <algorithm>
#include <boost/spirit/include/phoenix_function.hpp>


namespace parser {
  namespace phoenix = boost::phoenix;
  namespace qi = boost::spirit::qi;

  // enum allocation_operation_enum { add, sub, mul, div, pow, set, setv };

  const double pi = std::atan(1)*4;
  const double exp1 = std::exp(1);

  /*
  struct vec_alloc_lazy_impl {
    template <typename Arg1, typename Arg2, typename Arg3>
    struct result 
    { 
      typedef void type;
    };

    template <typename Arg1, typename Arg2, typename Arg3>
    void operator()(Arg1 index, Arg2 &val, Arg3 &arg) const
    {
      using namespace boost::phoenix::arg_names;

      switch (index) {
      case add:
	for_each(val.begin(), val.end(), arg1 += arg);
	break;
      case sub:
	for_each(val.begin(), val.end(), arg1 -= arg);
	break;
      case mul:
	for_each(val.begin(), val.end(), arg1 *= arg);
	break;
      case div:
	for_each(val.begin(), val.end(), arg1 /= arg);
	break;
      case pow:
	for_each(val.begin(), val.end(), pot_lazy(arg1, arg));
	break;
      case set:
	for_each(val.begin(), val.end(), arg1 = arg);
	break;
      case setv:
	for_each((val.begin(), arg.begin()), (val.end(), arg.end()), arg1 = arg2);
	break;
      default: ;
      }
    }
  };
  */

  struct func_symbol_struct : qi::symbols<char, unsigned> {
    func_symbol_struct() {
      add("sin", 1)
	("cos", 2)
	("tan", 3)
	("atan", 4)
	("arctan", 4)
	("exp", 5)
	("ln", 6)
	("lg", 7);
    }
  } func_symbol;

  // the first argument is a number defining the function
  // the second argument is the function's argument
  struct unary_symb_func_lazy_impl {
    template <typename Arg1, typename Arg2>
    struct result 
    { 
      typedef double type; 
    };

    template <typename Arg1, typename Arg2>
    double operator()(Arg1 index, Arg2 &arg) const
    {
      double res = -1;
      
      switch (index) {
      case 1: res = std::sin(arg); break;
      case 2: res = std::cos(arg); break;
      case 3: res = std::tan(arg); break;
      case 4: res = std::atan(arg); break;
      case 5: res = std::exp(arg); break;
      case 6: res = std::log(arg); break;
      case 7: res = std::log(arg)/log(10); break;
      default:break;
      }
      return res;
    }
  };

  struct pot_lazy_impl
  {
    template <typename Arg1, typename Arg2>
    struct result 
    { 
      typedef void type;
    };

    template <typename Arg1, typename Arg2>
    void operator()(Arg1 &base, Arg2 expo) const
    {
      base = std::pow(base, expo);
    }
  };

  phoenix::function<pot_lazy_impl> pot_lazy;
  phoenix::function<unary_symb_func_lazy_impl> unary_function_lazy;
    //  phoenix::function<vec_alloc_lazy_impl> vec_alloc;
}
#endif
