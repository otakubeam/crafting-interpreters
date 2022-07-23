#pragma once

#include <ast/syntax_tree.hpp>

#include <lex/lexer.hpp>

#include <catch2/catch.hpp>

class Parser {
 public:
  Parser(lex::Lexer l) : lexer_{l} {
  }

  Expression* ParseExpression() {
    return ParseComparison();
  }

  //   This area is used for literate prgramming
  //                         ===================
  //        Use it however you wish
  //        -----------------------
  //
  //
  //
  //
  //
  //
  //
  //
  //

  Expression* ParseComparison();
  Expression* ParseBinary();
  Expression* ParseUnary();
  Expression* ParsePrimary();

  ////////////////////////////////////////////////////////////////////

 private:
  bool Matches(lex::TokenType type) {
    if (lexer_.Peek().type != type) {
      return false;
    }

    lexer_.Advance();
    return true;
  }

 private:
  lex::Lexer lexer_;
};
