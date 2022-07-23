#include <ast/visitors/printing_visitor.hpp>

#include <parse/parse.hpp>

// Finally,
#include <catch2/catch.hpp>

//////////////////////////////////////////////////////////////////////

TEST_CASE("Just works", "[parser]") {
  char stream[] = "1 - 2";
  Parser p{lex::Lexer{stream}};

  // lex::Location dummy;
  // lex::Token tokens[] = {
  //     lex::Token{lex::TokenType::NUMBER, dummy, {1}},
  //     lex::Token{lex::TokenType::PLUS, dummy, {}},
  //     lex::Token{lex::TokenType::NUMBER, dummy, {2}},
  // };

  PrintingVisitor v;

  v.Eval(p.ParsePrimary());
  v.Eval(p.ParseUnary());
}
