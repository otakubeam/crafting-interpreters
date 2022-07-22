#pragma once

#include <ast/visitors/template_visitor.hpp>
#include <ast/syntax_tree.hpp>

#include <rt/base_object.hpp>

class Evaluator : public ReturnVisitor<SBObject> {
 public:

  virtual void VisitComparison(ComparisonExpression* node) {
    auto lhs = Eval(node->left_);
    auto rhs = Eval(node->right_);

    switch (node->operator_.type) {
       case lex::TokenType::EQ:
         return_value = {PrimitiveType{lhs == rhs}};
         break;
       case lex::TokenType::LT:
         // return_value = {PrimitiveType{lhs < rhs}};
         break;
       default:
         std::abort();   
    }
  }

  virtual void VisitBinary(BinaryExpression* node) {
    auto lhs = Eval(node->left_);
    auto rhs = Eval(node->right_);

    switch (node->operator_.type) {
       case lex::TokenType::PLUS:
          return_value = plus(lhs, rhs);
          break;
       case lex::TokenType::MINUS:
          return_value = minus(lhs, rhs);
          break;
       default:
          std::abort();
    }

  }

  virtual void VisitUnary(UnaryExpression* node) {
    auto val = Eval(node->operand_);

    switch (node->operator_.type) {
      case lex::TokenType::NOT:
         return_value = bang(val);
          break;
      case lex::TokenType::MINUS:
         return_value = negate(val);
          break;
      default:
        std::abort();
    }
  }

  virtual void VisitLiteral(LiteralExpression* lit) {
    auto val = lit->token_.sem_info;
    return_value = {PrimitiveType{val}};
  }

};
