#include <ast/visitors/evaluator.hpp>

#include <rt/base_object.hpp>

#include <parse/parse.hpp>

#include <iostream>

int main() {
  Evaluator e;
  Parser p(lex::Lexer{std::cin});

  while (auto stmt = p.ParseStatement()) {
      e.Eval(stmt);   
  }

  return 0;
}
