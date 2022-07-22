#include <parse/visitors/template_visitor.hpp>
#include <parse/syntax_tree.hpp>

class Evaluator : public ReturnVisitor<SBObject> {
 public:

    // Of course visit cannot return SBObject

  virtual void VisitComparison(ComparisonExpression* node) {
    
  }

  virtual void VisitBinary(BinaryExpression* node) {
    auto lhs = Eval(node->left_);
    auto rhs = Eval(node->right_);

    switch (node->operator_.type) {
       case lex::TokenType::PLUS:
          return_value = plus(rhs, lhs);
       case lex::TokenType::MINUS:
          return_value = minus(rhs, lhs);
       default:
          std::abort();
    }

  }

  virtual void VisitUnary(UnaryExpression* node) {
    auto val = Eval(node->operand_);

    switch (node->operator_.type) {
      case lex::TokenType::NOT:
         return_value = bang(val);
      case lex::TokenType::MINUS:
         return_value = negate(val);
      default:
        std::abort();
    }
  }

  virtual void VisitLiteral(LiteralExpression* lit) {
    auto val = lit->token_.sem_info;
    return_value = SBObject{PrimitiveType{val}};
  }
};
