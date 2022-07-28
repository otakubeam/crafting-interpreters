#pragma once

#include <ast/syntax_tree.hpp>
#include <ast/expressions.hpp>

#include <lex/token.hpp>

#include <vector>

//////////////////////////////////////////////////////////////////////

class Statement : public TreeNode {
 public:
  virtual void Accept(Visitor* /* visitor */){};
};

//////////////////////////////////////////////////////////////////////

class IfStatement : public Statement {
 public:
  IfStatement(Expression* condition, Statement* true_branch,
              Statement* false_branch = nullptr)
      : condition_{condition},
        true_branch_{true_branch},
        false_branch_{false_branch} {
  }

  virtual void Accept(Visitor* visitor) override {
    visitor->VisitIf(this);
  }

  Expression* condition_;
  Statement* true_branch_;
  Statement* false_branch_ = nullptr;
};

//////////////////////////////////////////////////////////////////////

class ExprStatement : public Statement {
 public:
  ExprStatement(Expression* expr) : expr_{expr} {
  }

  virtual void Accept(Visitor* visitor) override {
    visitor->VisitExpression(expr_);
  }

  Expression* expr_;
};

//////////////////////////////////////////////////////////////////////

class VarDeclStatement : public Statement {
 public:
  VarDeclStatement(LiteralExpression* lvalue, Expression* value)
      : lvalue_{lvalue}, value_{value} {
  }

  virtual void Accept(Visitor* visitor) override {
    visitor->VisitVarDecl(this);
  }

  // TODO: TypeExpression* type_;
  LiteralExpression* lvalue_;
  Expression* value_;
};

//////////////////////////////////////////////////////////////////////

// fun f(a1, a2, a3) { }

class FunDeclStatement : public Statement {
 public:
  FunDeclStatement(lex::Token name, std::vector<lex::Token> formals,
                   Statement* block)
      : name_{name}, formals_{std::move(formals)}, block_{block} {
  }

  virtual void Accept(Visitor* visitor) override {
    visitor->VisitFunDecl(this);
  }

  lex::Token name_;
  std::vector<lex::Token> formals_;
  Statement* block_;
};

//////////////////////////////////////////////////////////////////////

// fun f(a1, a2, a3) { }

class BlockStatement : public Statement {
 public:
  BlockStatement(std::vector<Statement*> stmts) : stmts_{stmts} {
  }

  virtual void Accept(Visitor* visitor) override {
    visitor->VisitBlockStatement(this);
  }

  std::vector<Statement*> stmts_;
};

//////////////////////////////////////////////////////////////////////
