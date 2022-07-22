#pragma once

#include <ast/visitors/template_visitor.hpp>
#include <ast/syntax_tree.hpp>

#include <fmt/format.h>

class PrintingVisitor : public ReturnVisitor<std::string> {
 public:
  virtual void VisitComparison(ComparisonExpression* node) {
    return_value = fmt::format(
        "Cmp-of-(\n"
        "\t{} < {}) \n",  //
        Eval(node->left_), Eval(node->right_));
  };

  virtual void VisitBinary(BinaryExpression* node) {
    return_value = fmt::format(
        "Binary-of-(\n"
        "\t{} \t + {})\n",  //
        Eval(node->left_), Eval(node->right_));
  }

  virtual void VisitUnary(UnaryExpression* node) {
    return_value = fmt::format(
        "Unary-of-(\n"
        "\t - {}) \n",  //
        Eval(node->operand_));
  }

  virtual void VisitLiteral(LiteralExpression* /* node */) {
    return_value = fmt::format("Literal 5\n");
  }
};
