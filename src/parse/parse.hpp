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

  ////////////////////////////////////////////////////////////////////

  Expression* ParseComparison() {
    Expression* fst = ParseBinary();

    UNSCOPED_INFO("Parsing the rest of comparison");

    using lex::TokenType;
    auto token = lexer_.Peek();

    while (Matches(TokenType::LT)) {
      if (auto snd = ParseBinary()) {
        fst = new ComparisonExpression(fst, token, snd);
      } else {
        throw "Incomplete Comparison Expression";
      }
    }

    return fst;
  }

  ////////////////////////////////////////////////////////////////////

  Expression* ParseBinary() {
    Expression* fst = ParseUnary();

    UNSCOPED_INFO("Parsing the rest of Binary");

    using lex::TokenType;
    auto token = lexer_.Peek();

    while (Matches(TokenType::PLUS) || Matches(TokenType::MINUS)) {
      if (auto snd = ParseUnary()) {
        fst = new BinaryExpression(fst, token, snd);
      } else {
        throw "Parse error: Incomplete Binary Expression";
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
        throw "Parse error: could not parse primary starting with minus";
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
    Expression* result = nullptr;

    // Try parsing grouping first

    if (Matches(lex::TokenType::LEFT_BRACE)) {
      if (auto expr = ParseExpression()) {
        if (Matches(lex::TokenType::RIGHT_BRACE)) {
          return expr;
        }
        throw "Parse error: missing right brace";
      }
      throw "Parse error: missing braced expression";
    }

    // Then all the base cases

    auto token = lexer_.Peek();

    switch (token.type) {
      case lex::TokenType::NUMBER:
      case lex::TokenType::STRING:
        result = new LiteralExpression{std::move(token)};
        break;

      case lex::TokenType::IDENTIFIER:
        std::abort();

      default:
        throw "Parse error: "
               "Could not match primary expression\n";
        break;
    }

    // Advance for all the base cases and return

    lexer_.Advance();
    return result;
  }

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
