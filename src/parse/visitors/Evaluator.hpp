#include <parse/SyntaxTree.hpp>

class Evaluator : public Visitor {
 public:
  virtual SBObject VisitComparison(ComparisonExpression* node) = 0;

  virtual SBObject VisitBinary(BinaryExpression* node) = 0;

  virtual SBObject VisitUnary(UnaryExpression* node) {
    auto val = node->operand_->Accept<SBObject>(this);
    switch (node->operator_.type) {
      case lex::TokenType::NOT:
      case lex::TokenType::MINUS:
      default:
        std::abort();
    }
  }

  virtual SBObject VisitLiteral(LiteralExpression* lit) {
    auto val = lit->token_.sem_info;
    return SBObject{PrimitiveType{val}};
  }
};
