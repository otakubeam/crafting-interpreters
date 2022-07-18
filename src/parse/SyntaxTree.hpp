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

class BinaryExpression : public Expression {
 public:
  BinaryExpression(Expression* left, lex::Token op, Expression* right)
      : left_{left}, operator_(op), right_{right} {
  }

  virtual void Accept(Visitor* visitor) override {
    visitor->VisitBinary(this);
  }

  Expression* left_;
  lex::Token operator_;
  Expression* right_;
};

//////////////////////////////////////////////////////////////////////

class UnaryExpression : public Expression {
 public:
  UnaryExpression(lex::Token op, Expression* operand)
      : operator_(op), operand_{operand} {
  }

  virtual void Accept(Visitor* visitor) override {
    visitor->VisitUnary(this);
  }

  lex::Token operator_;
  Expression* operand_;
};

//////////////////////////////////////////////////////////////////////

class LiteralExpression : public Expression {
 public:
  LiteralExpression(lex::Token token) : token_{token} {
  }

  virtual void Accept(Visitor* visitor) override {
    visitor->VisitLiteral(this);
  }

  lex::Token token_{};
};

//////////////////////////////////////////////////////////////////////

// TODO: It seems that grouping indeed dosn't need it own class
class GroupingExpression : public Expression {};

//////////////////////////////////////////////////////////////////////
