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
  Lexer(std::istream& source) : info_{source} {
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
    while (IsWhitespace(info_.CurrentSymbol())) {
      info_.MoveRight();
    }
  }

  void SkipComments() {
    while (info_.CurrentSymbol() == '#') {
      info_.MoveNextLine();
      SkipWhitespace();
    }
  }

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchOperators() {
    if (auto type = MatchSingleWidthOperator(info_.CurrentSymbol())) {
      info_.MoveRight();
      return Token{*type, info_.GetLocation()};
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

    while (isdigit(info_.CurrentSymbol())) {
      result *= 10;
      result += info_.CurrentSymbol() - '0';

      info_.MoveRight();
      match_span += 1;
    }

    if (match_span == 0) {
      return std::nullopt;
    }

    return Token{TokenType::NUMBER, info_.GetLocation(), {result}};
  }

  std::optional<Token> MatchStringLiteral() {
    auto first_quote = [](char first) -> bool {
      return first == '\"';
    };

    if (!first_quote(info_.CurrentSymbol())) {
      return std::nullopt;
    }

    // It matched! Now do match the whole string

    // Consume commencing "
    info_.MoveRight();

    std::string lit;
    while (info_.CurrentSymbol() != '\"') {
      lit.push_back(info_.CurrentSymbol());
      info_.MoveRight();
    }

    // Consume enclosing "
    info_.MoveRight();

    return Token{TokenType::STRING, info_.GetLocation(), {lit[0]}};
  }

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchWords() {
    auto word = BufferWord();
    auto type = table_.LookupOrInsert(word);

    if (type == TokenType::IDENTIFIER) {
      return Token{type, info_.GetLocation(), {word}};
    }

    // So it must be a keyword with the
    // exact type encoded direcly in `type`
    return Token{type, info_.GetLocation()};
  }

  std::string BufferWord() {
    std::string result;

    while (isalnum(info_.CurrentSymbol())) {
      result.push_back(info_.CurrentSymbol());
      info_.MoveRight();
    }

    return result;
  }

  ////////////////////////////////////////////////////////////////////

 private:
  Token peek_{};
  Scanner info_;
  IdentTable table_;
};

}  // namespace lex
