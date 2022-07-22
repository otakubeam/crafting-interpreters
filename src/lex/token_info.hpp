#pragma once

#include <lex/token_type.hpp>

#include <variant>
#include <string>

namespace lex {

//////////////////////////////////////////////////////////////////////

struct Location {
  int file = 0;
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
