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

TEST_CASE("Booleans", "[parser]") {
  char stream[] = "!true";
  Parser p{lex::Lexer{stream}};

  Evaluator e;
  CHECK(e.Eval(p.ParseExpression()) == FromPrim(false));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Variable declaration", "[parser]") {
  char stream[] = "var x = 5;";
  Parser p{lex::Lexer{stream}};
  CHECK_NOTHROW(p.ParseStatement());
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("Misleading minus", "[parser]") {
  char stream[] = "- 1 - 2";
  Parser p{lex::Lexer{stream}};

  Evaluator e;
  CHECK(e.Eval(p.ParseExpression()) == FromPrim(-3));
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("No left bracket", "[parser]") {
  char stream[] = "1 - (1 + 2";
  Parser p{lex::Lexer{stream}};
  CHECK_THROWS(p.ParseExpression());
}

//////////////////////////////////////////////////////////////////////

TEST_CASE("No braced expression", "[parser]") {
  char stream[] = "()";
  Parser p{lex::Lexer{stream}};
  CHECK_THROWS(p.ParseExpression());
}

//////////////////////////////////////////////////////////////////////
//                           Statements
//////////////////////////////////////////////////////////////////////

TEST_CASE("Expression statement", "[parser]") {
  char stream[] = "1 + 2;";
  Parser p{lex::Lexer{stream}};
  CHECK_NOTHROW(p.ParseStatement());
}

//////////////////////////////////////////////////////////////////////

// TEST_CASE("If statement", "[parser]") {
//   char stream[] = "if 1 = 1 1; else 2;";
//   Parser p{lex::Lexer{stream}};
//   CHECK_NOTHROW(p.ParseStatement());
// }

//////////////////////////////////////////////////////////////////////
