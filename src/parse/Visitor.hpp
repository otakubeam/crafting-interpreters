#pragma once

//////////////////////////////////////////////////////////////////////

class TreeNode;
class Expression;
class PrimaryExpression;

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
  virtual void VisitExpression(Expression* node) = 0;
  virtual void VisitPrimaryExpression(PrimaryExpression* node) = 0;
};

//////////////////////////////////////////////////////////////////////
