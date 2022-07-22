#include <parse/visitors/base_object.hpp>
#include <parse/visitors/evaluator.hpp>

#include <parse/parse.hpp>

#include <iostream>

int main() {
  lex::Location loc;
  auto ty = lex::TokenType::NUMBER;

  auto tk = lex::Token{ty, loc, 1}, tk2 = lex::Token{ty, loc, 2};
  auto a1 = LiteralExpression{tk}, a2 = LiteralExpression{tk2};

  auto minus_tk = lex::Token{lex::TokenType::MINUS, loc};
  auto plus_tk = lex::Token{lex::TokenType::PLUS, loc};

  auto un1 = UnaryExpression{minus_tk, &a1};
  auto bin1 = BinaryExpression{&a1, minus_tk, &a2};
  auto bin2 = BinaryExpression{&un1, plus_tk, &bin1};

  Evaluator e;  

  auto result2 = GetPrim<int>(e.Eval(&bin1));
  std::cout << result2 << std::endl;

  auto result = GetPrim<int>(e.Eval(&bin2));
  std::cout << result << std::endl;
}
