#pragma once

#include <lex/ident_table.hpp>
#include <lex/lexer_aux.hpp>
#include <lex/token.hpp>

#include <catch2/catch.hpp>
#include <fmt/format.h>

#include <optional>
#include <string>

namespace lex {

class Lexer {
 public:
  Lexer(std::istream& source) : scanner_{source} {
    Advance();  // so that it starts in a valid state for Peek()
  }

  ////////////////////////////////////////////////////////////////////

  Token GetNextToken() {
    SkipWhitespace();

    SkipComments();

    if (auto op = MatchOperators()) {
      return *op;
    }

    if (auto lit = MatchLiterls()) {
      return *lit;
    }

    if (auto word = MatchWords()) {
      return *word;
    }

    FMT_ASSERT(false, "\nCould not match any token\n");
  }

  ////////////////////////////////////////////////////////////////////

  Token Advance() {
    peek_ = GetNextToken();
    return peek_;
  }

  Token Peek() const {
    return peek_;
  }

  ////////////////////////////////////////////////////////////////////

 private:
  void SkipWhitespace() {
    while (IsWhitespace(scanner_.CurrentSymbol())) {
      scanner_.MoveRight();
    }
  }

  void SkipComments() {
    while (scanner_.CurrentSymbol() == '#') {
      scanner_.MoveNextLine();
      SkipWhitespace();
    }
  }

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchOperators() {
    if (auto type = MatchOperator()) {
      scanner_.MoveRight();
      return Token{*type, scanner_.GetLocation()};
    }

    return std::nullopt;
  }

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchLiterls() {
    if (auto num_token = MatchNumericLiteral()) {
      return num_token;
    }

    if (auto string_token = MatchStringLiteral()) {
      return string_token;
    }

    return std::nullopt;
  }

  std::optional<Token> MatchNumericLiteral() {
    int result = 0, match_span = 0;

    while (isdigit(scanner_.CurrentSymbol())) {
      result *= 10;
      result += scanner_.CurrentSymbol() - '0';

      scanner_.MoveRight();
      match_span += 1;
    }

    if (match_span == 0) {
      return std::nullopt;
    }

    return Token{TokenType::NUMBER, scanner_.GetLocation(), {result}};
  }

  std::optional<Token> MatchStringLiteral() {
    auto first_quote = [](char first) -> bool {
      return first == '\"';
    };

    if (!first_quote(scanner_.CurrentSymbol())) {
      return std::nullopt;
    }

    // It matched! Now do match the whole string

    // Consume commencing "
    scanner_.MoveRight();

    std::string lit;
    while (scanner_.CurrentSymbol() != '\"') {
      lit.push_back(scanner_.CurrentSymbol());
      scanner_.MoveRight();
    }

    // Consume enclosing "
    scanner_.MoveRight();

    return Token{TokenType::STRING, scanner_.GetLocation(), {lit}};
  }

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchWords() {
    auto word = BufferWord();
    auto type = table_.LookupOrInsert(word);

    if (type == TokenType::IDENTIFIER) {
      return Token{type, scanner_.GetLocation(), {word}};
    }

    // So it must be a keyword with the
    // exact type encoded direcly in `type`
    return Token{type, scanner_.GetLocation()};
  }

  std::string BufferWord() {
    std::string result;

    while (isalnum(scanner_.CurrentSymbol())) {
      result.push_back(scanner_.CurrentSymbol());
      scanner_.MoveRight();
    }

    return result;
  }

  ////////////////////////////////////////////////////////////////////
 private:
  std::optional<TokenType> MatchOperator() {
    switch (scanner_.CurrentSymbol()) {
        // A bit of a special case
      case '=': {
        if (scanner_.PeekNextSymbol() == '=') {
          scanner_.MoveRight();
          return TokenType::EQUALS;
        } else {
          return TokenType::ASSIGN;
        }
      }

      case '+':
        return TokenType::PLUS;
      case '-':
        return TokenType::MINUS;
      case '!':
        return TokenType::NOT;
      case '<':
        return TokenType::LT;
      case '(':
        return TokenType::LEFT_BRACE;
      case ')':
        return TokenType::RIGHT_BRACE;
      case '{':
        return TokenType::LEFT_CBRACE;
      case '}':
        return TokenType::RIGHT_CBRACE;
      case ';':
        return TokenType::SEMICOLUMN;
      case ',':
        return TokenType::COMMA;
      case EOF:
        return TokenType::TOKEN_EOF;
      default:
        return std::nullopt;
    }
  }

 private:
  Token peek_{};
  Scanner scanner_;
  IdentTable table_;
};

}  // namespace lex
