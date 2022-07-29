#pragma once

#include <rt/ifunction.hpp>

#include <fmt/core.h>

//////////////////////////////////////////////////////////////////////

struct Print : public IFunction {
  virtual SBObject Compute(EnvVisitor<SBObject>*,
                           std::vector<SBObject> args) override {
    fmt::print("\n\n");

    for (auto obj : args) {
      fmt::print("{} ", Format(obj));
    }

    fmt::print("\n\n");
    return {};
  };
};

//////////////////////////////////////////////////////////////////////
