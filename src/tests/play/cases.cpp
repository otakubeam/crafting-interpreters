#include <ast/visitors/printing_visitor.hpp>
#include <ast/visitors/evaluator.hpp>

#include <rt/base_object.hpp>

#include <parse/parse.hpp>

#include <lex/lexer.hpp>

#include <fmt/core.h>

// Finally,
#include <catch2/catch.hpp>

//////////////////////////////////////////////////////////////////////

TEST_CASE("Just works", "[play]") {
  Evaluator e;
  Parser p(lex::Lexer{std::cin});

  while (auto stmt = p.ParseStatement()) {
    e.Eval(stmt);
  }
}

//////////////////////////////////////////////////////////////////////
