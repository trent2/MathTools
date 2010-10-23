#include <list>
#include <map>
#include <string>

#include "MathFunction.hpp"
#include "functions_ginac.hpp"
#include "functor_builder.hpp"

using namespace functor;

MathFunction::MathFunction(const QColor &col) : mOrig_func_string(""), mNum_Func(0), mColor(col) {
  createReader();
}

MathFunction::MathFunction(const std::string &s, const QColor &col) : mOrig_func_string(s), mNum_Func(0), mColor(col) {
  createReader();
  parse();
}

MathFunction::~MathFunction() {
  delete mNum_Func;
}

bool MathFunction::parse() {
  bool ret = true;
  try {
    mFunc_term = (*reader)(mOrig_func_string);
    mNum_Func = numeric_function_builder::create_func(mFunc_term);
  } catch (std::invalid_argument& err) {
    ret = false;
  }

  return ret;
}

void MathFunction::setUStepsize(const double &d) {
  if(mNum_Func)
    mNum_Func->setStepsize(d);
}

double MathFunction::operator()(const double &d) const {
  return (*mNum_Func)(d);
}

// a GiNaC parser
GiNaC::parser *MathFunction::reader;
extern GiNaC::symbol symb_x, symb_e;

void MathFunction::createReader() {
  if(!reader) {
    GiNaC::symtab *table = new GiNaC::symtab;
    (*table)["x"] = symb_x;
    (*table)["e"] = symb_e;
    reader = new GiNaC::parser(*table);
    reader->strict = true;
  }
}
