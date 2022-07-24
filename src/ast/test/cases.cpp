#include <ast/visitors/printing_visitor.hpp>
#include <ast/visitors/evaluator.hpp>

#include <rt/base_object.hpp>

#include <parse/parse.hpp>

#include <lex/lexer.hpp>

#include <fmt/core.h>

// Finally,
#include <catch2/catch.hpp>

//////////////////////////////////////////////////////////////////////

TEST_CASE("Expressions", "[ast]") {
  auto ty = lex::TokenType::NUMBER;
  lex::Location loc_dummy;

  auto minus_tk = lex::Token{lex::TokenType::MINUS, loc_dummy};
  auto plus_tk = lex::Token{lex::TokenType::PLUS, loc_dummy};

  auto tk = lex::Token{ty, loc_dummy, 1},  //
      tk2 = lex::Token{ty, loc_dummy, 2};

  auto a1 = LiteralExpression{tk}, a2 = LiteralExpression{tk2};

  auto un1 = UnaryExpression{minus_tk, &a1};
  auto bin1 = BinaryExpression{&a1, minus_tk, &a2},
       bin2 = BinaryExpression{&un1, plus_tk, &bin1};

  Evaluator e;

  auto result1 = GetPrim<int>(e.Eval(&bin1));
  auto result2 = GetPrim<int>(e.Eval(&bin2));

  REQUIRE(result1 == -1);
  REQUIRE(result2 == -2);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Print tree", "[ast]") {
  auto ty = lex::TokenType::NUMBER;
  lex::Location loc_dummy;

  auto minus_tk = lex::Token{lex::TokenType::MINUS, loc_dummy};
  auto plus_tk = lex::Token{lex::TokenType::PLUS, loc_dummy};

  auto tk = lex::Token{ty, loc_dummy, 1},  //
      tk2 = lex::Token{ty, loc_dummy, 2};

  auto a1 = LiteralExpression{tk}, a2 = LiteralExpression{tk2};

  auto un1 = UnaryExpression{minus_tk, &a1};
  auto bin1 = BinaryExpression{&a1, minus_tk, &a2},
       bin2 = BinaryExpression{&un1, plus_tk, &bin1};

  PrintingVisitor p;

  auto result1 = p.Eval(&bin1);
  auto result2 = p.Eval(&bin2);

  fmt::print("{}\n\n\n{}", result1, result2);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Keep state", "[ast]") {
  char stream[] = "var x = 5;";

  Parser p{lex::Lexer{stream}};
  auto parsed = p.ParseStatement();

  INFO("Parsed successfully");

  Evaluator e;
  CHECK_NOTHROW(e.Eval(parsed));
}

//////////////////////////////////////////////////////////////////////
