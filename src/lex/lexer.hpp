#pragma once

#include <lex/ident_table.hpp>
#include <lex/lexer_aux.hpp>

#include <optional>
#include <string>
#include <cstdio>

#include <catch2/catch.hpp>
#include <fmt/format.h>

namespace lex {

class Lexer {
 public:
  Lexer(char* stream) : info_{stream} {
    Advance();  // so that it starts in a valid state for Peek()
  }

  ////////////////////////////////////////////////////////////////////

  Token GetNextToken() {
    SkipWhitespace();

    if (auto op = MatchOperators()) {
      UNSCOPED_INFO("Return oper\n");
      return *op;
    }

    if (auto lit = MatchLiterls()) {
      UNSCOPED_INFO("Return literal\n");
      return *lit;
    }

    if (auto word = MatchKeywords()) {
      return *word;
    }

    std::abort();
  }

  ////////////////////////////////////////////////////////////////////

  Token Advance() {
    peek_ = GetNextToken();
    return peek_;
  }

  Token Peek() const {
    return peek_;
  }

  ////////////////////////////////////////////////////////////////////

 private:
  void SkipWhitespace() {
    while (IsWhitespace(info_.CurrentSymbol())) {
      info_.MoveRight();
    }
  }

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchOperators() {
    if (auto type = MatchSingleWidthOperator(info_.CurrentSymbol())) {
      info_.MoveRight();
      return Token{*type, info_.GetSpan(1), {0}};
    }

    return std::nullopt;
  }

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchLiterls() {
    if (auto num_token = MatchNumericLiteral()) {
      UNSCOPED_INFO("Return numeric\n");

      return num_token;
    }

    if (auto string_token = MatchStringLiteral()) {
      return string_token;
    }

    return std::nullopt;
  }

  std::optional<Token> MatchNumericLiteral() {
    int result = 0, match_span = 0;

    while (isdigit(info_.CurrentSymbol())) {
      UNSCOPED_INFO(                                  //
          fmt::format("CurrentSymbol {} is a digit",  //
                      info_.CurrentSymbol()));

      result *= 10;
      result += info_.CurrentSymbol() - '0';

      info_.MoveRight();
      match_span += 1;
    }

    if (match_span == 0) {
      return std::nullopt;
    }

    return Token{TokenType::NUMBER, info_.GetSpan(match_span), {result}};
  }

  std::optional<Token> MatchStringLiteral() {
    if (info_.CurrentSymbol() != '\"') {
      return std::nullopt;
    }

    // It matched! Now do match the whole string

    info_.MoveRight();
    std::string lit;

    while (info_.CurrentSymbol() != '\"') {
      lit.push_back(info_.CurrentSymbol());
      info_.MoveRight();
    }

    return Token{TokenType::STRING, info_.GetSpan(lit.length()), {lit[0]}};
  }

  ////////////////////////////////////////////////////////////////////

  std::string BufferWord() {
    std::string result;

    while (isalnum(info_.CurrentSymbol())) {
      result.push_back(info_.CurrentSymbol());
      info_.MoveRight();
    }

    return result;
  }

  std::optional<Token> MatchKeywords() {
    auto word = BufferWord();
    auto type = table_.LookupOrInsert(word);
    if (type == TokenType::IDENTIFIER) {
      // TODO: gather identifiers with the same name
      return Token{type, info_.GetSpan(word.length()), {word[0]}};
    } else {
      return Token{type, info_.GetSpan(word.length()), {0}};
    }
  }

  ////////////////////////////////////////////////////////////////////

 private:
  Token peek_{};
  ScanInfo info_;
  IdentTable table_{};
};

}  // namespace lex
