#pragma once

#include <parse/visitors/BaseObject.hpp>

//////////////////////////////////////////////////////////////////////

class TreeNode;
class Expression;
class ComparisonExpression;
class BinaryExpression;
class UnaryExpression;
class LiteralExpression;

// TODO: should Visitor actually move out of `parse` dir?

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
  virtual void VisitComparison(ComparisonExpression* node) = 0;

  virtual void VisitBinary(BinaryExpression* node) = 0;

  virtual void VisitUnary(UnaryExpression* node) = 0;

  virtual void VisitLiteral(LiteralExpression* node) = 0;
};

//////////////////////////////////////////////////////////////////////
