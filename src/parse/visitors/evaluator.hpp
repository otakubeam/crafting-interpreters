#include <parse/visitors/template_visitor.hpp>
#include <parse/syntax_tree.hpp>

class Evaluator : public ReturnVisitor<SBObject> {
 public:

    // Of course visit cannot return SBObject

  virtual void VisitComparison(ComparisonExpression* node) = 0;

  virtual void VisitBinary(BinaryExpression* node) = 0;

  virtual void VisitUnary(UnaryExpression* node) {
    auto val = Eval(node->operand_);

    switch (node->operator_.type) {
      case lex::TokenType::NOT:
         return_value = SBObject{! std::get<bool>(val.object_)};
      case lex::TokenType::MINUS:
      default:
        std::abort();
    }
  }

  virtual void VisitLiteral(LiteralExpression* lit) {
    auto val = lit->token_.sem_info;
    return_value = SBObject{PrimitiveType{val}};
  }
};
