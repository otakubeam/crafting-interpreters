#include <parse/visitors/base_object.hpp>
#include <parse/visitors/evaluator.hpp>

#include <parse/parse.hpp>

#include <fmt/core.h>
#include <iostream>

bool TestEvaluator() {

  lex::Location loc_dummy;
  auto ty = lex::TokenType::NUMBER;

  auto minus_tk = lex::Token{lex::TokenType::MINUS, loc_dummy};
  auto plus_tk  = lex::Token{lex::TokenType::PLUS, loc_dummy};

  auto tk = lex::Token{ty, loc_dummy, 1},//
      tk2 = lex::Token{ty, loc_dummy, 2};

  auto a1 = LiteralExpression{tk}, //
       a2 = LiteralExpression{tk2};
  auto un1 = UnaryExpression{minus_tk, &a1};
  auto bin1 = BinaryExpression{&a1, minus_tk, &a2}, //
       bin2 = BinaryExpression{&un1, plus_tk, &bin1};

  Evaluator e;  

  auto result1 = GetPrim<int>(e.Eval(&bin1));
  fmt::print("Result is {}, expected is {}\n", result1, -1);

  auto result2 = GetPrim<int>(e.Eval(&bin2));
  fmt::print("Result is {}, expected is {}\n", result2, -2);

  return true;
}

int main() {
   TestEvaluator();
}
