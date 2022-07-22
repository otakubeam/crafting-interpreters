#pragma once

#include <lex/TokenInfo.hpp>

#include <string>
#include <map>

namespace lex {

class IdentTable {
 public:
  IdentTable() {
    Populate();
  }

  void Populate() {
    map_.insert({"print", TokenType::PRINT});
    map_.insert({"var", TokenType::VAR});
  }

  TokenType LookupOrInsert(const std::string& lexeme) {
    if (!map_.contains(lexeme)) {
      map_.insert({lexeme, TokenType::IDENTIFIER});
    }
    return map_[lexeme];
  }

 private:
  std::map<std::string, TokenType> map_;
};

}  // namespace lex
