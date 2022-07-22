#include <parse/visitors/base_object.hpp>
#include <parse/visitors/evaluator.hpp>

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

  REQUIRE(result1 < 0);
  REQUIRE(result2 == -2);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Just works", "[lex]") {
  char stream[] = "1 + 2";
  lex::Lexer l{stream};

  REQUIRE(l.Peek().type == lex::TokenType::NUMBER);
  CHECK(l.Advance().type == lex::TokenType::PLUS);
  CHECK(l.Advance().type == lex::TokenType::NUMBER);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Grouping", "[lex]") {
  char stream[] = "1 + (1)";
  lex::Lexer l{stream};

  REQUIRE(l.Peek().type == lex::TokenType::NUMBER);
  CHECK(l.Advance().type == lex::TokenType::PLUS);
  CHECK(l.Advance().type == lex::TokenType::LEFT_BRACE);
  REQUIRE(l.Advance().type == lex::TokenType::NUMBER);
  CHECK(l.Advance().type == lex::TokenType::RIGHT_BRACE);
}

TEST_CASE("Keywords", "[lex]") {
  char stream[] = "var fun for if";
  lex::Lexer l{stream};

  REQUIRE(l.Peek().type == lex::TokenType::VAR);
  CHECK(l.Advance().type == lex::TokenType::FUN);
  CHECK(l.Advance().type == lex::TokenType::FOR);
  CHECK(l.Advance().type == lex::TokenType::IF);
}
