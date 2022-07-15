#include <iostream>
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

void BufferMore(ScanInfo& info) {
  // ...
}

char CurrentSymbol(struct ScanInfo& info) {
  return info.stream_buf[info.loc.columnno];
}

bool IsWhitespace(char ch) {
  return ch == '\n' || ch == ' ' || ch == '\t';
}

void MoveRight(ScanInfo& info) {
  if (CurrentSymbol(info) == EOF) {
  }

  if (CurrentSymbol(info) == '\n') {
  } else {
  }
}

void SkipWhitespace(ScanInfo& info) {
  while (IsWhitespace(CurrentSymbol(info))) {
    ;
  }
}

//////////////////////////////////////////////////////////////////////

struct Token {
  enum TokenType type;
  struct Location loc;
  union {
    int value;
    std::string name;
  };
};

//////////////////////////////////////////////////////////////////////

struct Token NextToken(ScanInfo& info) {
  while (SkipWhitespace(info)) {
    ;
  }
}
