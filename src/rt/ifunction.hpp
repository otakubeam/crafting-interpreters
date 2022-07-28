#pragma once

#include <ast/visitors/template_visitor.hpp>

#include <rt/base_object.hpp>

#include <vector>

struct IFunction {
  virtual ~IFunction() = default;

  virtual SBObject Compute(ReturnVisitor<SBObject>* e,
                           std::vector<SBObject>) = 0;
};
