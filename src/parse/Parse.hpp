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

  TreeNode* ParsePrimary() {
    auto token = lexer_.GetNextToken();

    switch (token.type) {
      case lex::TokenType::NUMBER:
        return new PrimaryExpression{std::move(token)};
        break;
      case lex::TokenType::STRING:
        break;
      case lex::TokenType::IDENTIFIER:
        // unimplemented!
        std::abort();
      case lex::TokenType::LEFT_BRACE:
        if (auto expr = ParseExpression()) {
          // TODO: Match right BRACE
          // auto token = lexer_.GetNextToken();

          return expr;
        }
      case lex::TokenType::RIGHT_BRACE:

      // explicit default:
      case lex::TokenType::EQ:
      case lex::TokenType::LT:
      case lex::TokenType::TOKEN_EOL:
      case lex::TokenType::TOKEN_EOF:
      case lex::TokenType::PLUS:
      case lex::TokenType::MINUS:
      case lex::TokenType::NOT:
      case lex::TokenType::PRINT:
      case lex::TokenType::FUN:
      case lex::TokenType::VAR:
      case lex::TokenType::IF:
      case lex::TokenType::FOR:
        lexer_.UngetNextToken(token);
        return nullptr;
    }

    // unreachable!
    std::abort();
  }

  ////////////////////////////////////////////////////////////////////

 private:
  lex::Lexer lexer_;
};
