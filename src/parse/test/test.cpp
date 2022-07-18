#include <src/parse/Parse.hpp>

int main() {
  lex::Location loc;
  auto ty = lex::TokenType::NUMBER;

  auto tk = lex::Token{ty, loc, 1}, tk2 = lex::Token{ty, loc, 2};
  auto a1 = PrimaryExpression{tk}, a2 = PrimaryExpression{tk};
}
