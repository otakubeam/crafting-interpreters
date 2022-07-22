#pragma once

#include <ast/visitors/template_visitor.hpp>
#include <ast/visitors/evaluator.hpp>

#include <ast/syntax_tree.hpp>

#include <fmt/format.h>

class PrintingVisitor : public ReturnVisitor<std::string> {
 public:
  virtual void VisitComparison(ComparisonExpression* node) {
    return_value = fmt::format(
        "Comparison-of-(\n"
        "\t type:    {}  \n"
        "\t operand: {}) \n"                         //
        "\t operand: {}) \n",                        //
        lex::FormatTokenType(node->operator_.type),  //
        Eval(node->left_), Eval(node->right_));
  };

  virtual void VisitBinary(BinaryExpression* node) {
    return_value = fmt::format(
        "Binary-of-(\n"
        "\t type:    {}  \n"
        "\t operand: {}) \n"                         //
        "\t operand: {}) \n",                        //
        lex::FormatTokenType(node->operator_.type),  //
        Eval(node->left_), Eval(node->right_));
  }

  virtual void VisitUnary(UnaryExpression* node) {
    return_value = fmt::format(
        "Unary-of-(\n"
        "\t type:    {}  \n"
        "\t operand: {}) \n",                        //
        lex::FormatTokenType(node->operator_.type),  //
        Eval(node->operand_));
  }

  virtual void VisitLiteral(LiteralExpression* node) {
    auto lit_string = Format(literal_eval_.Eval(node));
    return_value = fmt::format("Literal {}", lit_string);
  }

 private:
  Evaluator literal_eval_;
};
