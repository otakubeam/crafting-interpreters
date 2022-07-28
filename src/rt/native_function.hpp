#pragma once

#include <ast/statements.hpp>

#include <rt/ifunction.hpp>

//////////////////////////////////////////////////////////////////////

struct FunctionType : public IFunction {
  FunctionType(FunDeclStatement* fn) : fn{fn} {
  }

  virtual SBObject Compute(ReturnVisitor<SBObject>* e,
                           std::vector<SBObject> args) override {
    return {};
  };

  FunDeclStatement* fn;
};

inline bool operator==(FunctionType lhs, FunctionType rhs) {
  return lhs.fn == rhs.fn;
}

//////////////////////////////////////////////////////////////////////
