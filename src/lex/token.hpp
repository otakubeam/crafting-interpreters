#pragma once

#include <lex/scanner.hpp>

#include <variant>
#include <cstddef>

namespace lex {

//////////////////////////////////////////////////////////////////////

struct Token {
  using SemInfo = std::variant<  //
      std::monostate,            //
      std::string,               //
      bool,                      //
      int                        //
      >;

  Token(TokenType type,  //
        Location start,  //
        SemInfo sem_info = {})
      : type{type}, start{start}, sem_info{sem_info} {
  }

  Token() = default;

  TokenType type;

  Location start;

  SemInfo sem_info = {};
};

//////////////////////////////////////////////////////////////////////

}  // namespace lex
