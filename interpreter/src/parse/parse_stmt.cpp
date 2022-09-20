#include <parse/parser.hpp>
#include <parse/parse_error.hpp>

///////////////////////////////////////////////////////////////////

Statement* Parser::ParseStatement() {
  if (auto struct_declaration = ParseStructDeclStatement()) {
    return struct_declaration;
  }

  if (auto var_declaration = ParseVarDeclStatement()) {
    return var_declaration;
  }

  if (auto fun_declaration = ParseFunDeclStatement()) {
    return fun_declaration;
  }

  if (auto return_statement = ParseReturnStatement()) {
    return return_statement;
  }

  if (auto yield_statement = ParseYieldStatement()) {
    return yield_statement;
  }

  if (auto expression_statement = ParseExprStatement()) {
    return expression_statement;
  }

  FMT_ASSERT(false, "Unreachable!");
}

///////////////////////////////////////////////////////////////////

FunDeclStatement* Parser::ParseFunDeclStatement() {
  if (!Matches(lex::TokenType::FUN)) {
    return nullptr;
  }

  auto function_name = lexer_.Peek();
  Consume(lex::TokenType::IDENTIFIER);

  auto typed_formals = ParseFormals();
  auto return_type = ParseType();

  if (auto block = dynamic_cast<BlockExpression*>(ParseBlockExpression())) {
    return new FunDeclStatement{function_name, return_type,
                                std::move(typed_formals), block};
  }

  throw parse::errors::ParseTrueBlockError{
      function_name.location.Format(),
  };
}

///////////////////////////////////////////////////////////////////

StructDeclStatement* Parser::ParseStructDeclStatement() {
  if (!Matches(lex::TokenType::STRUCT)) {
    return nullptr;
  }

  // 1. Get the name of the new struct

  auto struct_name = lexer_.Peek();
  Consume(lex::TokenType::IDENTIFIER);
  Consume(lex::TokenType::LEFT_CBRACE);

  // 2. Parse contents

  std::vector<lex::Token> fields;
  std::vector<types::Type*> types;

  while (Matches(lex::TokenType::IDENTIFIER)) {
    fields.push_back(lexer_.PreviousToken());
    Consume(lex::TokenType::COLUMN);

    if (auto type = ParseType()) {
      types.push_back(type);
    } else {
      throw parse::errors::ParseTypeError{FormatLocation()};
    }

    // Demand the trailing comma!
    Consume(lex::TokenType::COMMA);
  }

  Consume(lex::TokenType::RIGHT_CBRACE);
  Consume(lex::TokenType::SEMICOLUMN);

  return new StructDeclStatement{struct_name, std::move(fields),
                                 std::move(types)};
}

///////////////////////////////////////////////////////////////////

auto Parser::ParseFormals() -> std::vector<FunDeclStatement::FormalParam> {
  Consume(lex::TokenType::LEFT_BRACE);

  std::vector<FunDeclStatement::FormalParam> typed_formals;

  while (Matches(lex::TokenType::IDENTIFIER)) {
    auto param_name = lexer_.PreviousToken();
    Consume(lex::TokenType::COLUMN);

    if (auto type = ParseType()) {
      typed_formals.push_back(FunDeclStatement::FormalParam{
          .ident = param_name,
          .type = type,
      });
    } else {
      throw parse::errors::ParseTypeError{FormatLocation()};
    }

    if (!Matches(lex::TokenType::COMMA)) {
      break;
    }
  }

  Consume(lex::TokenType::RIGHT_BRACE);
  return typed_formals;
}

///////////////////////////////////////////////////////////////////

ReturnStatement* Parser::ParseReturnStatement() {
  if (!Matches(lex::TokenType::RETURN)) {
    return nullptr;
  }

  auto location_token = lexer_.PreviousToken();

  Expression* ret_expr = nullptr;
  if (!Matches(lex::TokenType::SEMICOLUMN)) {
    ret_expr = ParseExpression();
    Consume(lex::TokenType::SEMICOLUMN);
  }

  return new ReturnStatement{location_token, ret_expr};
}

///////////////////////////////////////////////////////////////////

YieldStatement* Parser::ParseYieldStatement() {
  if (!Matches(lex::TokenType::YIELD)) {
    return nullptr;
  }

  auto location_token = lexer_.PreviousToken();

  Expression* yield_value = nullptr;
  if (!Matches(lex::TokenType::SEMICOLUMN)) {
    yield_value = ParseExpression();
    Consume(lex::TokenType::SEMICOLUMN);
  }

  return new YieldStatement{location_token, yield_value};
}

///////////////////////////////////////////////////////////////////

VarDeclStatement* Parser::ParseVarDeclStatement() {
  if (!Matches(lex::TokenType::VAR)) {
    return nullptr;
  }

  // 1. Get a name to assign to

  Consume(lex::TokenType::IDENTIFIER);
  auto lvalue = new VarAccessExpression{lexer_.PreviousToken()};

  // 2. Get an expression to assign to

  Consume(lex::TokenType::ASSIGN);

  auto value = ParseExpression();

  Consume(lex::TokenType::SEMICOLUMN);

  return new VarDeclStatement{lvalue, value};
}

///////////////////////////////////////////////////////////////////

Statement* Parser::ParseExprStatement() {
  auto expr = ParseExpression();

  if (Matches(lex::TokenType::ASSIGN)) {
    if (auto target = dynamic_cast<LvalueExpression*>(expr)) {
      return ParseAssignment(target);
    }

    throw parse::errors::ParseNonLvalueError{FormatLocation()};
  }

  try {
    Consume(lex::TokenType::SEMICOLUMN);
  } catch (...) {
    // So that the last expression in block can be caught
    //   {
    //     stmt;
    //     stmt;   <<<--- parse all of it as statements
    //     expr           but catch the last one
    //   }
    //
    throw new ExprStatement{expr};
  }

  return new ExprStatement{expr};
}

AssignmentStatement* Parser::ParseAssignment(LvalueExpression* target) {
  auto assignment_loc = lexer_.PreviousToken();
  auto value = ParseExpression();
  Consume(lex::TokenType::SEMICOLUMN);
  return new AssignmentStatement{assignment_loc, target, value};
}
