#pragma once

#include <ast/syntax_tree.hpp>

#include <lex/lexer.hpp>

#include <catch2/catch.hpp>

#include <vector>

class Parser {
 public:
  Parser(lex::Lexer l) : lexer_{l} {
  }

  Expression* ParseExpression() {
    auto token = lexer_.GetNextToken();

    while (token.type != lex::TokenType::TOKEN_EOF) {
    }

    std::abort();

    return new Expression{};
  }

  ////////////////////////////////////////////////////////////////////

  // Only for syntactic ones
  bool Matches(lex::TokenType type) {
    if (lexer_.Peek().type != type) {
      return false;
    }

    lexer_.Advance();
    return true;
  }

  ////////////////////////////////////////////////////////////////////

  Expression* ParseComparison() {
    Expression* fst = ParseBinary();
    auto tk = lexer_.Peek();

    using lex::TokenType;
    while (Matches(TokenType::LT)) {
      if (auto snd = ParseBinary()) {
        fst = new ComparisonExpression(fst, tk, snd);
      } else {
        throw "Incomplete Comparison Expression";
      }
    }

    return fst;
  }

  ////////////////////////////////////////////////////////////////////

  Expression* ParseBinary() {
    Expression* fst = ParseUnary();

    UNSCOPED_INFO("Parsing the rest");

    using lex::TokenType;
    auto tk = lexer_.Peek();

    while (Matches(TokenType::PLUS) || Matches(TokenType::MINUS)) {
      if (auto snd = ParseUnary()) {
        fst = new BinaryExpression(fst, tk, snd);
      } else {
        throw "Incomplete Binary Expression";
      }
    }

    return fst;
  }

  ////////////////////////////////////////////////////////////////////

  Expression* ParseUnary() {
    auto token = lexer_.Peek();

    if (Matches(lex::TokenType::MINUS)) {
      if (auto expr = ParsePrimary()) {
        return new UnaryExpression{token, expr};
      } else {
        throw "Parse Error: could not parse primary starting with minus";
      }
    }

    auto expr = ParsePrimary();
    FMT_ASSERT(expr,
               "\n Parse error: "
               "Could not match Unary Expression \n");

    return expr;
  }

  ////////////////////////////////////////////////////////////////////

  Expression* ParsePrimary() {
    auto token = lexer_.Peek();

    Expression* result = nullptr;

    // Try parsing grouping first

    if (Matches(lex::TokenType::LEFT_BRACE)) {
      if (auto expr = ParseExpression()) {
        if (Matches(lex::TokenType::RIGHT_BRACE)) {
          result = expr;
        }
        throw "Parse error: missing right brace";
      }
      throw "Parse error: missing braced expression";
    }

    // Then all the other base cases

    switch (token.type) {
      case lex::TokenType::NUMBER:
      case lex::TokenType::STRING:
        result = new LiteralExpression{std::move(token)};
        break;

      case lex::TokenType::IDENTIFIER:
        std::abort();

      default:
        UNSCOPED_INFO("Could not match anything in Primary Rule");
        break;
    }

    FMT_ASSERT(result,
               "\nError: "
               "Could not match primary expression\n");

    lexer_.Advance();
    return result;
  }

  /////////////////////////////////////////////////////////////////////

 public:
  bool SelfTest() {
    return true;
  }

 private:
  lex::Lexer lexer_;
};
