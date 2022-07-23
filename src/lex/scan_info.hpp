#pragma once

#include <lex/token_type.hpp>

#include <filesystem>

namespace lex {

//////////////////////////////////////////////////////////////////////

struct Location {
  size_t lineno = 0;
  size_t columnno = 0;
};

//////////////////////////////////////////////////////////////////////

struct SpanLines {
  Location start;
  size_t span_number;
};

//////////////////////////////////////////////////////////////////////

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
        // End stop
        // Don't support several files
        break;

      default:
        loc.columnno += 1;
    }
  }

  char CurrentSymbol() const {
    return stream_buf[loc.columnno];
  }

  Location GetLocation() const {
    return loc;
  }

 private:
  char* stream_buf = 0;
  Location loc;
};

//////////////////////////////////////////////////////////////////////

}  // namespace lex
