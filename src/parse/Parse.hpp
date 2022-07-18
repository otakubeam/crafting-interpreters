#pragma once

#include <src/parse/SyntaxTree.hpp>

#include <src/lex/Lexer.hpp>

#include <vector>

class Parser {
 public:
  TreeNode* ParseExpression() {
    auto token = lexer_.GetNextToken();

    while (token.type != lex::TokenType::TOKEN_EOF) {
    }

    return new Expression{};
  }

  ////////////////////////////////////////////////////////////////////

  // Only for syntactic ones
  bool Match(lex::TokenType type) {
    if (lexer_.Peek().type != type) {
      return false;
    }

    // TODO: advance()
    // lexer_.GetNextToken()
  }

  TreeNode* ParsePrimary() {
    auto token = lexer_.Peek();

    switch (token.type) {
      case lex::TokenType::NUMBER:
      case lex::TokenType::STRING:
        return new PrimaryExpression{std::move(token)};

      case lex::TokenType::IDENTIFIER:
        // unimplemented!
        std::abort();

      case lex::TokenType::LEFT_BRACE:
        if (auto expr = ParseExpression()) {
          // TODO: Match right BRACE
          // auto token = lexer_.GetNextToken();

          return expr;
        }

      default:
        // TODO: report errors
        return nullptr;
    }
  }

  ////////////////////////////////////////////////////////////////////

 private:
  lex::Lexer lexer_;
};
