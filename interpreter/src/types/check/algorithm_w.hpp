#pragma once

#include <types/type.hpp>
#include <types/trait.hpp>

#include <ast/scope/context.hpp>

#include <ast/visitors/template_visitor.hpp>

#include <queue>

namespace types::check {

class AlgorithmW : public ReturnVisitor<Type*> {
 public:
  AlgorithmW() {
  }

  void VisitYield(YieldStatement* node);
  void VisitReturn(ReturnStatement* node);
  void VisitAssignment(AssignmentStatement* node);
  void VisitExprStatement(ExprStatement* node);

  void VisitTypeDecl(TypeDeclStatement* node);
  void VisitVarDecl(VarDeclStatement* node);
  void VisitFunDecl(FunDeclStatement* node);
  void VisitTraitDecl(TraitDeclaration* node);

  void VisitComparison(ComparisonExpression* node);
  void VisitBinary(BinaryExpression* node);
  void VisitUnary(UnaryExpression* node);
  void VisitDeref(DereferenceExpression* node);
  void VisitAddressof(AddressofExpression* node);
  void VisitIf(IfExpression* node);
  void VisitNew(NewExpression* node);
  void VisitBlock(BlockExpression* node);
  void VisitFnCall(FnCallExpression* node);
  void VisitIntrinsic(IntrinsicCall* node);
  void VisitFieldAccess(FieldAccessExpression* node);
  void VisitTypecast(TypecastExpression* node);
  void VisitLiteral(LiteralExpression* node);
  void VisitVarAccess(VarAccessExpression* node);
  void VisitCompoundInitalizer(CompoundInitializerExpr* node);

 private:
  void PushEqual(Type* a, Type* b);

 private:
  std::deque<Trait> deferred_checks_;
};

}  // namespace types::check
