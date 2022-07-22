#include <lex/lexer.hpp>

// Finally,
#include <catch2/catch.hpp>

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

//////////////////////////////////////////////////////////////////////

TEST_CASE("Keywords", "[lex]") {
  char stream[] = "var fun for if print";
  lex::Lexer l{stream};

  REQUIRE(l.Peek().type == lex::TokenType::VAR);
  CHECK(l.Advance().type == lex::TokenType::FUN);
  CHECK(l.Advance().type == lex::TokenType::FOR);
  CHECK(l.Advance().type == lex::TokenType::IF);
  CHECK(l.Advance().type == lex::TokenType::PRINT);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Comments", "[lex]") {
  // TODO: string literals inside comments?
  char stream[] = "// comment var a = 5 ; + - = fun // \n 1";
  lex::Lexer l{stream};

  // parses to just `1`
  REQUIRE(l.Peek().type == lex::TokenType::NUMBER);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Statement", "[lex]") {
  char stream[] = "var abc = 0;";
  lex::Lexer l{stream};

  CHECK(l.Peek().type == lex::TokenType::VAR);
  CHECK(l.Advance().type == lex::TokenType::IDENTIFIER);
  CHECK(l.Advance().type == lex::TokenType::EQ);
  CHECK(l.Advance().type == lex::TokenType::NUMBER);
  CHECK(l.Advance().type == lex::TokenType::COLUMN);
}

//////////////////////////////////////////////////////////////////////
