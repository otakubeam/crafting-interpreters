#pragma once

#include <parse/visitor.hpp>
#include <parse/syntax_tree.hpp>

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
