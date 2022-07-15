#include <iostream>
#include <string>
#include <optional>

#include <src/TokenInfo.hpp>

class Lexer {
 public:
  Token NextToken() {
    SkipWhitespace();

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

  void SkipWhitespace() {
    while (IsWhitespace(info_.CurrentSymbol())) {
      ;
    }
  }

  std::optional<Token> MatchOperators() {
  }

 private:
  ScanInfo info_;
};
