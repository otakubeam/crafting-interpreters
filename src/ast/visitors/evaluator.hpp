#pragma once

#include <ast/visitors/template_visitor.hpp>

#include <ast/expressions.hpp>
#include <ast/statements.hpp>

#include <rt/primitive_type.hpp>
#include <rt/base_object.hpp>

#include <unordered_map>

class Evaluator : public ReturnVisitor<SBObject> {
 public:
  virtual void VisitIf(IfStatement* node) override {
    auto cond = GetPrim<bool>(Eval(node->condition_));
    auto* branch = cond ? node->true_branch_ : node->false_branch_;
    Eval(branch);
  }

  virtual void VisitStatement(Statement* /* node */) override {
    FMT_ASSERT(false, "Visiting bare statement");
  }

  virtual void VisitVarDecl(VarDeclStatement* node) override {
    // TODO: need a table to store the state
    auto name = std::get<std::string>(node->lvalue_->token_.sem_info);
    auto val = Eval(node->value_);
    state_.emplace(name, val);
  }

  virtual void VisitExprStatement(ExprStatement* node) override {
    Eval(node->expr_);
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitExpression(Expression* /* node */) override {
    FMT_ASSERT(false, "Visiting bare expression");
  }

  virtual void VisitComparison(ComparisonExpression* node) override {
    auto lhs = Eval(node->left_);
    auto rhs = Eval(node->right_);

    switch (node->operator_.type) {
      case lex::TokenType::EQ:
        return_value = {PrimitiveType{lhs == rhs}};
        break;

      case lex::TokenType::LT:
        // return_value = {PrimitiveType{lhs < rhs}};
        std::abort();
        break;

      default:
        std::abort();
    }
  }

  virtual void VisitBinary(BinaryExpression* node) override {
    auto lhs = Eval(node->left_);
    auto rhs = Eval(node->right_);

    switch (node->operator_.type) {
      case lex::TokenType::PLUS:
        return_value = Plus(lhs, rhs);
        break;

      case lex::TokenType::MINUS:
        return_value = Minus(lhs, rhs);
        break;

      default:
        std::abort();
    }
  }

  virtual void VisitUnary(UnaryExpression* node) override {
    auto val = Eval(node->operand_);

    switch (node->operator_.type) {
      case lex::TokenType::NOT:
        return_value = Bang(val);
        break;

      case lex::TokenType::MINUS:
        return_value = Negate(val);
        break;

      default:
        std::abort();
    }
  }

  virtual void VisitLiteral(LiteralExpression* lit) override {
    switch (lit->token_.type) {
      case lex::TokenType::TRUE:
        return_value = {PrimitiveType{true}};
        break;

      case lex::TokenType::FALSE:
        return_value = {PrimitiveType{false}};
        break;

      default:
        auto val = lit->token_.sem_info;
        return_value = {FromSemInfo(val)};
    }
  }

 private:
  // TODO: Translation from SemInfo to AST nodes
  // should have happened during parsing
  PrimitiveType FromSemInfo(lex::Token::SemInfo sem_info) {
    switch (sem_info.index()) {
        // std::monostate
      case 0:
        throw "Error: evaluating monostate literal";

        // std::string
      case 1:
        // TODO: Make string a PrimitiveType
        return PrimitiveType{std::get<std::string>(sem_info)[0]};

        // bool
      case 2:
        FMT_ASSERT(false, "\n Error: Unreachable \n");

        // int
      case 3:
        return PrimitiveType{std::get<int>(sem_info)};

      default:
        FMT_ASSERT(false, "\n Error: Non-exhaustive match \n");
    }
  }

 private:
  using Name = std::string;

  std::unordered_map<Name, SBObject> state_;
};

//////////////////////////////////////////////////////////////////////
