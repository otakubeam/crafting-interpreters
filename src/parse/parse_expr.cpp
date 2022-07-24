#include <parse/parse.hpp>

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseComparison() {
  Expression* fst = ParseBinary();

  UNSCOPED_INFO("Parsing the rest of comparison");

  using lex::TokenType;
  auto token = lexer_.Peek();

  while (Matches(TokenType::LT)) {
    if (auto snd = ParseBinary()) {
      fst = new ComparisonExpression(fst, token, snd);
    } else {
      throw "Incomplete Comparison Expression";
    }
  }

  return fst;
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseBinary() {
  Expression* fst = ParseUnary();

  UNSCOPED_INFO("Parsing the rest of Binary");

  using lex::TokenType;
  auto token = lexer_.Peek();

  while (Matches(TokenType::PLUS) || Matches(TokenType::MINUS)) {
    if (auto snd = ParseUnary()) {
      fst = new BinaryExpression(fst, token, snd);
    } else {
      throw "Parse error: Incomplete Binary Expression";
    }
  }

  return fst;
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseUnary() {
  auto token = lexer_.Peek();

  if (Matches(lex::TokenType::MINUS) || Matches(lex::TokenType::NOT)) {
    if (auto expr = ParsePrimary()) {
      return new UnaryExpression{token, expr};
    } else {
      throw "Parse error: could not parse primary starting with minus";
    }
  }

  auto expr = ParsePrimary();
  FMT_ASSERT(expr,
             "\n Parse error: "
             "Could not match Unary Expression \n");

  return expr;
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParsePrimary() {
  Expression* result = nullptr;

  // Try parsing grouping first

  if (Matches(lex::TokenType::LEFT_BRACE)) {
    if (auto expr = ParseExpression()) {
      if (Matches(lex::TokenType::RIGHT_BRACE)) {
        return expr;
      }
      throw "Parse error: missing right brace";
    }
    throw "Parse error: missing braced expression";
  }

  // Then all the base cases

  auto token = lexer_.Peek();

  switch (token.type) {
    case lex::TokenType::IDENTIFIER:
    case lex::TokenType::NUMBER:
    case lex::TokenType::STRING:
    case lex::TokenType::FALSE:
    case lex::TokenType::TRUE:
      result = new LiteralExpression{std::move(token)};
      break;

    // case lex::TokenType::IDENTIFIER:
    default:
      throw "Parse error: "
               "Could not match primary expression\n";
      break;
  }

  // Advance for all the base cases and return

  lexer_.Advance();
  return result;
}

////////////////////////////////////////////////////////////////////
