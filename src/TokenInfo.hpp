#pragma once

#include <variant>

#include <string>

//////////////////////////////////////////////////////////////////////

// There is no behaviour to a token,
// so it doesn't make sense for it to be
// a class with virtual methods and such

enum class TokenType {
  NUMBER,
  STRING,

  PLUS,
  MINUS,

  PRINT,
  FUN,  // FUN foo
  VAR,  // VAR name
  // Wait, what?? name and foo should instead be its own things
  // perhaps TokenType ID
  IDENTIFIER,

  // These only change the state of the lexer
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
  ScanInfo() {
  }

  void BufferMore(char* new_buffer) {
    stream_buf = new_buffer;
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

  // char Peek() cosnt { }

  char CurrentSymbol() const {
    return stream_buf[loc.columnno];
  }

  Location CurrentTokenLocation(size_t length) const {
    return loc;
  }

 private:
  char* stream_buf = 0;
  Location loc;
};

//////////////////////////////////////////////////////////////////////

struct Token {
  TokenType type;

  Location loc;

  std::variant<int, std::string> sem_info{0};
};

//////////////////////////////////////////////////////////////////////
