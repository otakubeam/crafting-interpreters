#pragma once

#include <ast/syntax_tree.hpp>
#include <ast/expressions.hpp>

#include <lex/token.hpp>

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
