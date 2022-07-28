#pragma once

#include <rt/base_object.hpp>

//////////////////////////////////////////////////////////////////////

class TreeNode;

class Expression;
//---------------
class ComparisonExpression;
class BinaryExpression;
class UnaryExpression;
class LiteralExpression;
//---------------
class Statement;
class IfStatement;
class ExprStatement;
class VarDeclStatement;
class FunDeclStatement;

class Visitor {
  // So visitor must know about expressions and others
  // (so it can implement its own functionality)
  //
  // and tree nodes must know about visitor's existence
  // (so they can implement double dispatching)

  // But that's dinamic dispatch! Didn't he say we
  // only need static dispath for visitor?

  // Maybe I actually don't need it, but then I still would
  // have to define all the functionality of the visiotrs but
  // then then can have different names.

 public:
  // Statements

  virtual void VisitIf(IfStatement* node) = 0;

  virtual void VisitStatement(Statement* node) = 0;

  virtual void VisitVarDecl(VarDeclStatement* node) = 0;

  virtual void VisitFunDecl(FunDeclStatement* node) = 0;

  virtual void VisitExprStatement(ExprStatement* node) = 0;

  // Expressions

  virtual void VisitExpression(Expression* node) = 0;

  virtual void VisitComparison(ComparisonExpression* node) = 0;

  virtual void VisitBinary(BinaryExpression* node) = 0;

  virtual void VisitUnary(UnaryExpression* node) = 0;

  virtual void VisitLiteral(LiteralExpression* node) = 0;
};

//////////////////////////////////////////////////////////////////////
