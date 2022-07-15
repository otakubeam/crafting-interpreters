#include <iostream>
#include <string>

#include "TokenInfo.hpp"

class Lexer {
 public:
  Token NextToken(ScanInfo& info) {
    SkipWhitespace(info);

    // TODO: match operators
    // TODO: match keywords
    // TODO: match literals
    // TODO: match identifiers

    std::abort();
  }

 private:
  bool IsWhitespace(char ch) {
    return ch == '\n' || ch == ' ' || ch == '\t';
  }

  char CurrentSymbol(struct ScanInfo& info) {
    return info.stream_buf[info.loc.columnno];
  }

  void SkipWhitespace(ScanInfo& info) {
    while (IsWhitespace(CurrentSymbol(info))) {
      ;
    }
  }

 private:
};

//////////////////////////////////////////////////////////////////////

void BufferMore(ScanInfo& info) {
  // ...
}

void MoveRight(ScanInfo& info) {
  if (CurrentSymbol(info) == EOF) {
  }

  if (CurrentSymbol(info) == '\n') {
  } else {
  }
}
