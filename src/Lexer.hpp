#pragma once

#include <src/LexerStrCmp.hpp>

#include <src/TokenInfo.hpp>

#include <optional>
#include <string>

class Lexer {
 public:
  Token NextToken() {
    SkipWhitespace();

    if (auto op = MatchOperators()) {
    }
    // TODO: match keywords
    // TODO: match literals
    // TODO: match identifiers

    std::abort();
  }

 private:
  void SkipWhitespace() {
    while (IsWhitespace(info_.CurrentSymbol())) {
      info_.MoveRight();
    }
  }

  std::optional<int> abc() {
    return 5;
  }

  std::optional<Token> MatchOperators() {
    switch (info_.CurrentSymbol()) {
      case '+':
        auto t = Token{TokenType::PLUS, info_.CurrentTokenLocation(1), {0}};
        break;
      case '-':
      case '=':
      case '!':
      case '<':
      case '(':
      case ')':
      default:
        return std::nullopt;
    }
  }

 private:
  ScanInfo info_{};
};
