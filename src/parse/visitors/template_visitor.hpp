#pragma once

#include <parse/Visitor.hpp>
#include <parse/SyntaxTree.hpp>

template <typename T>
class ReturnVisitor {
   public:
      T Eval(Expression* expr) {
         expr->Accept(this);
         return return_value;
      }

   private:
      T return_value;
};
