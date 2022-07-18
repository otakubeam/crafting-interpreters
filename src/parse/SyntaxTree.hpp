#pragma once

#include <src/parse/Visitor.hpp>

#include <src/lex/TokenInfo.hpp>

//////////////////////////////////////////////////////////////////////

class TreeNode {
 public:
  virtual void Accept(Visitor* visitor) = 0;
};

//////////////////////////////////////////////////////////////////////

class Expression : public TreeNode {
  virtual void Accept(Visitor* visitor) override {
    visitor->VisitExpression(this);
  }
};

//////////////////////////////////////////////////////////////////////

class UnaryExpression : public Expression {
 public:
  lex::Token operator_;
  Expression* operand_;
};

//////////////////////////////////////////////////////////////////////

class PrimaryExpression : public Expression {
 public:
  virtual void Accept(Visitor* visitor) override {
    visitor->VisitPrimaryExpression(this);
  }

  // std::variant<LiteralExpression, GroupingExpression> primary_;

  // TODO: what if primary is a grouping expression?
};

class LiteralExpression : public Expression {
  LiteralExpression(lex::Token token) : token_{token} {
  }

  virtual void Accept(Visitor* visitor) override {
    visitor->VisitPrimaryExpression(this);
  }

  lex::Token token_{};
};

class GroupingExpression : public Expression {};
//////////////////////////////////////////////////////////////////////
