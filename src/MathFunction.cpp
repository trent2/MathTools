#include <list>
#include <map>
#include <string>

//#include "MathFunction.hpp"

#include "functions_ginac.hpp"
#include "functor_builder.hpp"

using namespace functor;

template <typename T>
MathFunction<T>::MathFunction(const std::string &s, const QColor &col) : mOrig_func_string(s),  mNum_Func(0), mColor(col), mParseOk(false) {
  createReader();
  parse();
}

template <typename T>
MathFunction<T>::~MathFunction() {
  delete mNum_Func;
}

template <typename T>
void MathFunction<T>::setFunction(const std::string &s) {
  mOrig_func_string = s;
  parse();
}

template <typename T>
T MathFunction<T>::operator()(const T &d) const {
  return (*mNum_Func)(d);
}

template <typename T>
void MathFunction<T>::setUStepsize(const double &d) {
  if(mNum_Func)
    mNum_Func->setStepsize(d);
}

// a GiNaC parser
template <typename T>
GiNaC::parser *MathFunction<T>::reader;
extern GiNaC::symbol symb_x, symb_e, symb_z, symb_i;

template <typename T>
void MathFunction<T>::createReader() {
  if(!reader) {
    GiNaC::symtab *table = new GiNaC::symtab;
    (*table)["x"] = symb_x;
    (*table)["z"] = symb_z;
    (*table)["i"] = symb_i;
    (*table)["e"] = symb_e;
    reader = new GiNaC::parser(*table);
    reader->strict = true;
  }
}
template <typename T>
void MathFunction<T>::parse() {
  mParseOk = true;
  try {
    mFunc_term = (*reader)(mOrig_func_string);
    mNum_Func = numeric_function_builder<T>::create_func(mFunc_term);
  } catch (std::exception& err) {
    mParseOk = false;
  }
}
