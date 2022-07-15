#pragma once

#include <src/TokenInfo.hpp>

#include <optional>

bool IsWhitespace(char ch) {
  return ch == '\n' || ch == ' ' || ch == '\t';
}

std::optional<TokenType> MatchSingleWidthOperator(char ch) {
  switch (ch) {
    case '+':
      return TokenType::PLUS;
    case '-':
      return TokenType::MINUS;
    case '=':
      return TokenType::EQ;
    case '!':
      return TokenType::NOT;
    case '<':
      return TokenType::LT;
    case '(':
      return TokenType::LBR;
    case ')':
      return TokenType::RBR;
    default:
      return std::nullopt;
  }
}
