#pragma once

#include <rt/ifunction.hpp>

#include <fmt/core.h>

//////////////////////////////////////////////////////////////////////

struct Print : public IFunction {
  virtual SBObject Compute(EnvVisitor<SBObject>*,
                           std::vector<SBObject>) override {
     fmt::print("Hello world");
     return {};
  };
};

//////////////////////////////////////////////////////////////////////
