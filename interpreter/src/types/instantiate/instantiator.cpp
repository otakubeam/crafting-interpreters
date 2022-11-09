#include <types/instantiate/instantiator.hpp>
#include <types/constraints/solver.hpp>

#include <lex/token.hpp>

namespace types::check {

//////////////////////////////////////////////////////////////////////

void TemplateInstantiator::VisitTypeDecl(TypeDeclStatement* node) {
  auto n = new TypeDeclStatement{*node};
  n->body_ = Instantinate(n->body_, poly_to_mono_);
  SetTyContext(n->body_, node->body_->typing_context_);

  return_value = n;
}

//////////////////////////////////////////////////////////////////////

void TemplateInstantiator::VisitVarDecl(VarDeclStatement* node) {
  auto n = new VarDeclStatement{*node};

  n->annotation_ = Instantinate(n->annotation_, poly_to_mono_);
  SetTyContext(n->annotation_, node->layer_);

  n->value_ = Eval(n->value_)->as<Expression>();

  return_value = n;
}

//////////////////////////////////////////////////////////////////////

void TemplateInstantiator::VisitFunDecl(FunDeclStatement* node) {
  auto n = new FunDeclStatement{*node};
  n->type_ = Instantinate(n->type_, poly_to_mono_);
  SetTyContext(n->type_, FindLeader(node->type_)->typing_context_);

  n->body_ = Eval(n->body_)->as<Expression>();

  return_value = n;
}

//////////////////////////////////////////////////////////////////////

void TemplateInstantiator::VisitYield(YieldStatement* node) {
  auto n = new YieldStatement{*node};
  n->yield_value_ = Eval(n->yield_value_)->as<Expression>();

  return_value = n;
}

void TemplateInstantiator::VisitReturn(ReturnStatement* node) {
  auto n = new ReturnStatement{*node};
  n->return_value_ = Eval(n->return_value_)->as<Expression>();

  return_value = n;
}

void TemplateInstantiator::VisitAssignment(AssignmentStatement* node) {
  auto n = new AssignmentStatement{*node};

  n->target_ = Eval(n->target_)->as<LvalueExpression>();
  n->value_ = Eval(n->value_)->as<Expression>();

  return_value = n;
}

void TemplateInstantiator::VisitExprStatement(ExprStatement* node) {
  auto n = new ExprStatement{*node};
  n->expr_ = Eval(n->expr_)->as<Expression>();

  return_value = n;
}

//////////////////////////////////////////////////////////////////////

void TemplateInstantiator::VisitComparison(ComparisonExpression* node) {
  auto n = new ComparisonExpression{*node};

  n->left_ = Eval(n->left_)->as<Expression>();
  n->right_ = Eval(n->right_)->as<Expression>();

  return_value = n;
}

void TemplateInstantiator::VisitBinary(BinaryExpression* node) {
  auto n = new BinaryExpression{*node};


  n->left_ = Eval(n->left_)->as<Expression>();
  n->right_ = Eval(n->right_)->as<Expression>();

  n->type_ = n->left_->GetType();
  // SetTyContext(n->type_, FindLeader(node->type_)->typing_context_);

  return_value = n;
}

void TemplateInstantiator::VisitUnary(UnaryExpression* node) {
  auto n = new UnaryExpression{*node};
  n->operand_ = Eval(n->operand_)->as<Expression>();

  return_value = n;
}

void TemplateInstantiator::VisitDeref(DereferenceExpression* node) {
  auto n = new DereferenceExpression{*node};

  n->type_ = Instantinate(n->type_, poly_to_mono_);
  SetTyContext(n->type_, node->layer_);

  n->operand_ = Eval(n->operand_)->as<Expression>();

  return_value = n;
}

void TemplateInstantiator::VisitAddressof(AddressofExpression* node) {
  auto n = new AddressofExpression{*node};

  n->type_ = Instantinate(n->type_, poly_to_mono_);
  SetTyContext(n->type_, node->layer_);

  n->operand_ = Eval(n->operand_)->as<Expression>();

  return_value = n;
}

void TemplateInstantiator::VisitIf(IfExpression* node) {
  auto n = new IfExpression{*node};

  n->condition_ = Eval(n->condition_)->as<Expression>();
  n->true_branch_ = Eval(n->true_branch_)->as<Expression>();
  n->false_branch_ = Eval(n->false_branch_)->as<Expression>();

  return_value = n;
}

void TemplateInstantiator::VisitNew(NewExpression* node) {
  auto n = new NewExpression{*node};

  n->type_ = Instantinate(node->type_, poly_to_mono_);
  SetTyContext(n->type_, node->type_->typing_context_);

  n->underlying_ = Instantinate(node->underlying_, poly_to_mono_);
  SetTyContext(n->underlying_, node->underlying_->typing_context_);

  if (auto alloc = n->allocation_size_) {
    alloc = Eval(n->allocation_size_)->as<Expression>();
  }

  return_value = n;
}

void TemplateInstantiator::VisitBlock(BlockExpression* node) {
  auto n = new BlockExpression{*node};

  for (auto& s : n->stmts_) {
    s = Eval(s)->as<Statement>();
  }

  if (n->final_) {
    n->final_ = Eval(n->final_)->as<Expression>();
  }

  return_value = n;
}

void TemplateInstantiator::VisitFnCall(FnCallExpression* node) {
  auto n = new FnCallExpression{*node};

  for (auto& a : n->arguments_) {
    a = Eval(a)->as<Expression>();
  }

  // TODO: do I need this?
  // n->callable_ = Eval(node->callable_)->as<Expression>();

  n->callable_type_ = Instantinate(node->callable_type_, poly_to_mono_);

  auto leader = FindLeader(node->callable_type_);
  SetTyContext(n->callable_type_, leader->typing_context_);

  fmt::print("{}\n", FormatType(*n->callable_type_));
  fmt::print("Adding a node to the queue\n");

  instantiation_quque_.push_back(n);

  return_value = n;
}

void TemplateInstantiator::VisitIntrinsic(IntrinsicCall* node) {
  auto n = new IntrinsicCall{*node};
  for (auto& a : n->arguments_) {
    a = Eval(a)->as<Expression>();
  }
  return_value = n;
}

void TemplateInstantiator::VisitCompoundInitalizer(
    CompoundInitializerExpr* node) {
  auto n = new CompoundInitializerExpr{*node};

  for (auto& mem : n->initializers_) {
    mem.init = Eval(mem.init)->as<Expression>();
  }

  n->type_ = Instantinate(node->type_, poly_to_mono_);
  SetTyContext(n->type_, node->GetType()->typing_context_);

  return_value = n;
}

void TemplateInstantiator::VisitFieldAccess(FieldAccessExpression* node) {
  auto n = new FieldAccessExpression{*node};
  n->struct_expression_ = Eval(node->struct_expression_)->as<Expression>();

  n->type_ = Instantinate(node->type_, poly_to_mono_);
  SetTyContext(n->type_, node->layer_);

  return_value = n;
}

void TemplateInstantiator::VisitVarAccess(VarAccessExpression* node) {
  auto n = new VarAccessExpression{*node};

  n->type_ = Instantinate(node->GetType(), poly_to_mono_);
  SetTyContext(n->type_, node->layer_);

  return_value = n;
}

void TemplateInstantiator::VisitLiteral(LiteralExpression* node) {
  return_value = new LiteralExpression{*node};
}

void TemplateInstantiator::VisitTypecast(TypecastExpression* node) {
  auto n = new TypecastExpression{*node};

  n->type_ = Instantinate(node->type_, poly_to_mono_);
  SetTyContext(n->type_, node->GetType()->typing_context_);

  return_value = n;
}

}  // namespace types::check
