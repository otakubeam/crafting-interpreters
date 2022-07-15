#pragma once

#include <src/LexerStrCmp.hpp>

#include <src/TokenInfo.hpp>

#include <optional>
#include <string>

#include <stdio.h>

class Lexer {
 public:
  Token NextToken() {
    SkipWhitespace();

    if (auto op = MatchOperators()) {
      return *op;
    }

    if (auto lit = MatchLiterls()) {
      return *lit;
    }

    // TODO: match keywords
    // TODO: match identifiers

    std::abort();
  }

 private:
  void SkipWhitespace() {
    while (IsWhitespace(info_.CurrentSymbol())) {
      info_.MoveRight();
    }
  }

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchOperators() {
    if (auto type = MatchSingleWidthOperator(info_.CurrentSymbol())) {
      return Token{*type, info_.GetSpan(1), {0}};
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

    return Token{TokenType::NUMBER, info_.GetSpan(match_span), {result}};
  }

  std::optional<Token> MatchStringLiteral() {
    // TODO: where do I place string literals?
    return std::nullopt;
  }

  ////////////////////////////////////////////////////////////////////

 private:
  ScanInfo info_{};
};
