#pragma once

#include <string>

//////////////////////////////////////////////////////////////////////

enum TokenType {
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
  char* stream_buf;
  struct Location loc;
};

//////////////////////////////////////////////////////////////////////

struct Token {
  TokenType type;

  Location loc;

  union {
    int value;
    std::string name;
  } whatsmore;
};

//////////////////////////////////////////////////////////////////////
