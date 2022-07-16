#pragma once

#include <src/lex/LexerStrCmp.hpp>
#include <src/lex/IdentTable.hpp>
#include <src/lex/TokenInfo.hpp>

#include <optional>
#include <string>

#include <cstdio>

class Lexer {
 public:
  Token GetNextToken() {
    SkipWhitespace();

    if (auto op = MatchOperators()) {
      return *op;
    }

    if (auto lit = MatchLiterls()) {
      return *lit;
    }

    if (auto word = MatchKeywords()) {
      return *word;
    }

    std::abort();
  }

 private:
  void SkipWhitespace() {
    while (IsWhitespace(info_.CurrentSymbol())) {
      info_.MoveRight();
    }
  }

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchOperators() {
    if (auto type = MatchSingleWidthOperator(info_.CurrentSymbol())) {
      return Token{*type, info_.GetSpan(1), {0}};
    }

    return std::nullopt;
  }

  ////////////////////////////////////////////////////////////////////

  std::optional<Token> MatchLiterls() {
    if (auto num_token = MatchNumericLiteral()) {
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
      result *= 10;
      result += info_.CurrentSymbol() - '0';

      info_.MoveRight();
      match_span += 1;
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
    }

    return Token{TokenType::STRING, info_.GetSpan(lit.length()), {lit}};
  }

  ////////////////////////////////////////////////////////////////////

  std::string BufferWord() {
    std::string result;

    while (isalnum(info_.CurrentSymbol())) {
      result.push_back(info_.CurrentSymbol());
    }

    return result;
  }

  std::optional<Token> MatchKeywords() {
    auto word = BufferWord();
    auto type = table_.LookupOrInsert(word);
    if (type == TokenType::IDENTIFIER) {
      // TODO: gather identifiers with the same name
      return Token{type, info_.GetSpan(word.length()), {word}};
    } else {
      return Token{type, info_.GetSpan(word.length()), {0}};
    }
  }

  ////////////////////////////////////////////////////////////////////

 private:
  ScanInfo info_{};
  IdentTable table_{};
};
