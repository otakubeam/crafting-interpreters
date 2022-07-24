#pragma once

#include <cstdlib>

namespace lex {

//////////////////////////////////////////////////////////////////////

enum class TokenType {
  NUMBER,
  STRING,
  IDENTIFIER,

  // TODO:
  TRUE,
  FALSE,

  PLUS,
  MINUS,

  EQ,
  LT,

  LEFT_BRACE,
  RIGHT_BRACE,

  NOT,

  PRINT,
  FUN,  // FUN foo
  VAR,  // VAR name

  IF,
  ELSE,
  FOR,

  SEMICOLUMN,

  // These only change the location info of the lexer
  TOKEN_EOL,
  TOKEN_EOF,
};

////////////////////////////////////////////////////////////////

// clang-format off
#define AST_NODE_LIST(code) \
  code(NUMBER)              \
  code(STRING)              \
  code(IDENTIFIER)          \
  code(TRUE)                \
  code(FALSE)               \
  code(PLUS)                \
  code(MINUS)               \
  code(EQ)                  \
  code(LT)                  \
  code(LEFT_BRACE)          \
  code(RIGHT_BRACE)         \
  code(NOT)                 \
  code(PRINT)               \
  code(FUN )                \
  code(VAR)                 \
  code(IF)                  \
  code(ELSE)                \
  code(FOR)                 \
  code(SEMICOLUMN)          \
  code(TOKEN_EOL)           \
  code(TOKEN_EOF)
// clang-format on

////////////////////////////////////////////////////////////////

// https://journal.stuffwithstuff.com/2012/01/24/higher-order-macros-in-c/

#define DEFINE_TYPE_STRING(type) \
  case TokenType::type:          \
    return #type;

// There are just several places where this is used
inline const char* FormatTokenType(TokenType type) {
  switch (type) {
    AST_NODE_LIST(DEFINE_TYPE_STRING)
    default:
      break;
  }
  std::abort();
}

#undef DEFINE_TYPE_STRING

////////////////////////////////////////////////////////////////

}  // namespace lex
