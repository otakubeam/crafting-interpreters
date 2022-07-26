#pragma once

#include <lex/token_type.hpp>

#include <optional>
#include <istream>

namespace lex {

inline bool IsWhitespace(char ch) {
  return ch == '\n' || ch == ' ' || ch == '\t';
}

inline std::optional<TokenType> MatchSingleWidthOperator(char ch) {
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
      return TokenType::LEFT_BRACE;
    case ')':
      return TokenType::RIGHT_BRACE;
    case ';':
      return TokenType::SEMICOLUMN;
    case EOF:
      return TokenType::TOKEN_EOF;
    default:
      return std::nullopt;
  }
}

}  // namespace lex
