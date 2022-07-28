#pragma once

#include <ast/visitors/template_visitor.hpp>

#include <ast/expressions.hpp>
#include <ast/statements.hpp>

#include <rt/scope/environment.hpp>
#include <rt/primitive_type.hpp>
#include <rt/base_object.hpp>

class Evaluator : public ReturnVisitor<SBObject> {
 public:
  Evaluator();
  virtual ~Evaluator();

  ////////////////////////////////////////////////////////////////////

  virtual void VisitStatement(Statement* /* node */) override {
    FMT_ASSERT(false, "Visiting bare statement");
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitIf(IfStatement* node) override {
    auto cond = GetPrim<bool>(Eval(node->condition_));
    auto* branch = cond ? node->true_branch_ : node->false_branch_;
    Eval(branch);
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitVarDecl(VarDeclStatement* node) override {
    auto name = std::get<std::string>(node->lvalue_->token_.sem_info);
    auto val = Eval(node->value_);
    env_->Declare(name, val);
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitFunDecl(FunDeclStatement* node) override {
    auto name = std::get<std::string>(node->name_.sem_info);
    SBObject val = {FunctionType{node}};
    env_->Declare(name, val);
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitExprStatement(ExprStatement* node) override {
    Eval(node->expr_);
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitBlockStatement(BlockStatement* node) override {
    Environment::ScopeGuard guard{&env_};
    for (auto stmt : node->stmts_) {
      Eval(stmt);
    }
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitExpression(Expression* node) override;

  virtual void VisitComparison(ComparisonExpression* node) override;
  virtual void VisitBinary(BinaryExpression* node) override;
  virtual void VisitUnary(UnaryExpression* node) override;
  virtual void VisitLiteral(LiteralExpression* lit) override;

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
  Environment global_environment;
  Environment* env_{nullptr};
};
