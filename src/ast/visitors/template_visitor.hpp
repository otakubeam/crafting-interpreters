#pragma once

#include <ast/visitors/visitor.hpp>
#include <ast/syntax_tree.hpp>

template <typename T>
class ReturnVisitor : public Visitor {
   public:
      T Eval(Expression* expr) {
         expr->Accept(this);
         return return_value;
      }

   protected:
      T return_value;
};
