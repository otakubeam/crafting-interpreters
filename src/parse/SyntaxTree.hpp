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

class UnaryExpression : public TreeNode {};

//////////////////////////////////////////////////////////////////////

class PrimaryExpression : public TreeNode {
 public:
  PrimaryExpression(lex::Token token) : token_{token} {
  }

  virtual void Accept(Visitor* visitor) override {
    visitor->VisitPrimaryExpression(this);
  }

  lex::Token token_{};
};

//////////////////////////////////////////////////////////////////////
