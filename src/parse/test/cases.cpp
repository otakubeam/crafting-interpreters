#include <ast/visitors/printing_visitor.hpp>
#include <ast/visitors/evaluator.hpp>

#include <parse/parse.hpp>

// Finally,
#include <catch2/catch.hpp>

//////////////////////////////////////////////////////////////////////

TEST_CASE("Just works", "[parser]") {
  char stream[] = "1 - 2";
  Parser p{lex::Lexer{stream}};

  Evaluator e;
  CHECK(e.Eval(p.ParseExpression()) == FromPrim(-1));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Parse as separate", "[parser]") {
  char stream[] = "1 - 2";
  Parser p{lex::Lexer{stream}};

  Evaluator e;
  CHECK(e.Eval(p.ParsePrimary()) == FromPrim(1));
  CHECK(e.Eval(p.ParseUnary()) == FromPrim(-2));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Associativity", "[parser]") {
  char stream[] = "1 - 2 - 3";
  Parser p{lex::Lexer{stream}};

  Evaluator e;
  CHECK(e.Eval(p.ParseExpression()) == FromPrim(-4));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Grouping", "[parser]") {
  char stream[] = "1 - (2 - 3)";
  Parser p{lex::Lexer{stream}};

  Evaluator e;
  CHECK(e.Eval(p.ParseExpression()) == FromPrim(2));
}

//////////////////////////////////////////////////////////////////////
