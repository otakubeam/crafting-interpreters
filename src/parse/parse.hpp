#pragma once

#include <parse/SyntaxTree.hpp>

#include <lex/Lexer.hpp>

#include <vector>

class Parser {
 public:
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
    auto tk = lexer_.Peek();
    using lex::TokenType;

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

    switch (token.type) {
      case lex::TokenType::MINUS:
        if (auto expr = ParsePrimary()) {
          return new UnaryExpression{token, expr};
        }

      default:
        throw "Could not parse? In MY predictive parser???";
    }
  }

  ////////////////////////////////////////////////////////////////////

  // These are named for the grammar rules!
  // It's ok
  Expression* ParsePrimary() {
    auto token = lexer_.Peek();

    switch (token.type) {
      case lex::TokenType::NUMBER:
      case lex::TokenType::STRING:
        return new LiteralExpression{std::move(token)};

      case lex::TokenType::IDENTIFIER:
        // unimplemented!
        std::abort();

      case lex::TokenType::LEFT_BRACE:
        if (auto expr = ParseExpression()) {
          if (Matches(lex::TokenType::RIGHT_BRACE)) {
            return expr;
          }

          throw "Parse error: missing right brace";
        }

      default:
        // TODO: report errors
        return nullptr;
    }
  }

  ////////////////////////////////////////////////////////////////////

 private:
  lex::Lexer lexer_;
};
