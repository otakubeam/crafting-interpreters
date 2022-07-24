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

///////////////////////////////////////////////////////////////////

TEST_CASE("Keywords", "[lex]") {
  char stream[] = "var fun for if else print true false";
  lex::Lexer l{stream};

  REQUIRE(l.Peek().type == lex::TokenType::VAR);
  CHECK(l.Advance().type == lex::TokenType::FUN);
  CHECK(l.Advance().type == lex::TokenType::FOR);
  CHECK(l.Advance().type == lex::TokenType::IF);
  CHECK(l.Advance().type == lex::TokenType::ELSE);
  CHECK(l.Advance().type == lex::TokenType::PRINT);
  CHECK(l.Advance().type == lex::TokenType::TRUE);
  CHECK(l.Advance().type == lex::TokenType::FALSE);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Consequent", "[lex]") {
  char stream[] = "!true";
  lex::Lexer l{stream};

  CHECK(l.Peek().type == lex::TokenType::NOT);
  CHECK(l.Advance().type == lex::TokenType::TRUE);
}

//////////////////////////////////////////////////////////////////////

// TEST_CASE("Comments", "[lex]") {
//   char stream[] = "# Comment \n 1";
//   lex::Lexer l{stream};
//
//   // parses to just `1`
//   REQUIRE(l.Peek().type == lex::TokenType::NUMBER);
// }

//////////////////////////////////////////////////////////////////////

TEST_CASE("Statement", "[lex]") {
  char stream[] = "var abc = 0;";
  lex::Lexer l{stream};

  CHECK(l.Peek().type == lex::TokenType::VAR);
  CHECK(l.Advance().type == lex::TokenType::IDENTIFIER);
  CHECK(l.Advance().type == lex::TokenType::EQ);
  CHECK(l.Advance().type == lex::TokenType::NUMBER);
  CHECK(l.Advance().type == lex::TokenType::SEMICOLUMN);
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("String literal", "[lex]") {
  char stream[] = "\"Hello world\"";
  lex::Lexer l{stream};

  CHECK(l.Peek().type == lex::TokenType::STRING);
}

//////////////////////////////////////////////////////////////////////
