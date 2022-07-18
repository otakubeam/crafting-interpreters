#include <src/parse/Parse.hpp>

int main() {
  lex::Location loc;
  auto ty = lex::TokenType::NUMBER;

  auto tk = lex::Token{ty, loc, 1}, tk2 = lex::Token{ty, loc, 2};
  auto a1 = LiteralExpression{tk}, a2 = LiteralExpression{tk};

  auto minus_tk = lex::Token{lex::TokenType::MINUS, loc};
  auto plus_tk = lex::Token{lex::TokenType::PLUS, loc};

  auto un1 = UnaryExpression{minus_tk, &a1};
  auto bin1 = BinaryExpression{&a1, minus_tk, &a2};
  auto bin2 = BinaryExpression{&un1, plus_tk, &bin1};
}
