#include <parse/parser.hpp>

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseComparison() {
  Expression* fst = ParseBinary();

  using lex::TokenType;
  auto token = lexer_.Peek();

  while (Matches(TokenType::LT) || Matches(TokenType::EQUALS)) {
    if (auto snd = ParseBinary()) {
      fst = new ComparisonExpression(fst, token, snd);
    } else {
      // TODO: here I can catch the expection from the lower level
      // and throw the more informative one instead
      throw ParseError{"Incomplete Comparison Expression"};
    }
  }

  return fst;
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseBinary() {
  Expression* fst = ParseUnary();

  using lex::TokenType;
  auto token = lexer_.Peek();

  while (Matches(TokenType::PLUS) || Matches(TokenType::MINUS)) {
    if (auto snd = ParseUnary()) {
      fst = new BinaryExpression(fst, token, snd);
    } else {
      // TODO: here I can catch the expection from the lower level
      // and throw the more informative one instead
      throw ParseError{"Incomplete Binary Expression"};
    }
  }

  return fst;
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseUnary() {
  auto token = lexer_.Peek();

  if (Matches(lex::TokenType::MINUS) || Matches(lex::TokenType::NOT)) {
    if (auto expr = ParseIfExpression()) {
      return new UnaryExpression{token, expr};
    } else {
      throw ParseError{"Could not parse primary starting with minus"};
    }
  }

  if (auto expr = ParseIfExpression()) {
    return expr;
  } else {
    throw ParseError{"Could not match Unary Expression \n"};
  }
}

///////////////////////////////////////////////////////////////////

Expression* Parser::ParseIfExpression() {
  if (!Matches(lex::TokenType::IF)) {
    return ParseBlockExpression();
  }

  // This should be fine even without parentheses, right?
  auto condition = ParseExpression();
  AssertParsed(condition,  //
               "If statement without condition");

  auto true_branch = ParseBlockExpression();
  AssertParsed(true_branch,  //
               "If statement without true branch");

  Expression* false_branch = nullptr;

  if (Matches(lex::TokenType::ELSE)) {
    false_branch = ParseBlockExpression();
    AssertParsed(false_branch,  //
                 "Else clause without an associated statement");
  }

  return new IfExpression(condition, true_branch, false_branch);
}

///////////////////////////////////////////////////////////////////

Expression* Parser::ParseBlockExpression() {
  if (!Matches(lex::TokenType::LEFT_CBRACE)) {
    return ParseFunApplication();
  }

  std::vector<Statement*> stmts;
  Expression* final_expr = nullptr;

  while (!Matches(lex::TokenType::RIGHT_CBRACE)) {
    try {
      auto stmt = ParseStatement();
      stmts.push_back(stmt);
    } catch (ExprStatement* e) {
      final_expr = e->expr_;
      Consume(lex::TokenType::RIGHT_CBRACE);
      break;
    }
  }

  return new BlockExpression{std::move(stmts), final_expr};
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseFunApplication() {
  auto fun_name = lexer_.Peek();

  if (!Matches(lex::TokenType::IDENTIFIER)) {
    return ParsePrimary();
  }

  // At this point we have already consumed the token
  // Now it's our responsibility
  if (!Matches(lex::TokenType::LEFT_BRACE)) {
    return new LiteralExpression{std::move(fun_name)};
  }

  std::vector<Expression*> args;

  // Parse csv

  if (!Matches(lex::TokenType::RIGHT_BRACE)) {
    while (auto expr = ParseExpression()) {
      args.push_back(expr);
      if (!Matches(lex::TokenType::COMMA)) {
        break;
      }
    }

    Consume(lex::TokenType::RIGHT_BRACE);
  }

  return new FnCallExpression{fun_name, std::move(args)};
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
      throw ParseError{"Missing right brace\n"};
    }
    std::abort();  // unreachable
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

    default:
      throw ParseError{"Could not match primary expression\n"};
  }

  // Advance for all the base cases and return

  lexer_.Advance();
  return result;
}

////////////////////////////////////////////////////////////////////