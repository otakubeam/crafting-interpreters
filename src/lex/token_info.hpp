#pragma once

#include <variant>
#include <string>

namespace lex {

//////////////////////////////////////////////////////////////////////

// There is no behaviour to a token,
// so it doesn't make sense for it to be
// a class with virtual methods and such

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
  FOR,

  // These only change the location info of the lexer
  TOKEN_EOL,
  TOKEN_EOF,
};

//////////////////////////////////////////////////////////////////////

struct Location {
  int file = 0;

  // What token can span more that a line?
  // None, right?

  int lineno = 0;
  int columnno = 0;
};

struct ScanInfo {
 public:
  ScanInfo(char* new_buffer) : stream_buf{new_buffer} {
  }

  void MoveRight() {
    switch (CurrentSymbol()) {
      case '\n':
        loc.columnno = 0;
        loc.lineno += 1;
        break;

      case EOF:
        loc.file = -1;
        break;

      default:
        loc.columnno += 1;
    }
  }

  char CurrentSymbol() const {
    return stream_buf[loc.columnno];
  }

  Location GetSpan(size_t length) const {
    auto result = loc;

    // TODO: get an actual span
    result.columnno -= length;

    return result;
  }

 private:
  char* stream_buf = 0;
  Location loc;
};

//////////////////////////////////////////////////////////////////////

struct String {
  char* str = 0;
  size_t length = 0;
};

struct Token {
  TokenType type;

  Location loc;

  std::variant<        //
      std::nullptr_t,  //
      int,             //
      bool,            //
      char             //
      >
      sem_info{0};

  // TODO: optional identifier
};

//////////////////////////////////////////////////////////////////////

}  // namespace lex
