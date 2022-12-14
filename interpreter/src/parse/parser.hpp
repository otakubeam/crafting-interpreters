#pragma once

#include <driver/module.hpp>

#include <lex/lexer.hpp>

class Parser {
 public:
  Parser(lex::Lexer& l);

  auto ParseModule() -> Module;

  ///////////////////////////////////////////////////////////////////

  Statement* ParseStatement();

  ReturnStatement* ParseReturnStatement();
  YieldStatement* ParseYieldStatement();
  Statement* ParseExprStatement();
  AssignmentStatement* ParseAssignment(LvalueExpression* target);

  ////////////////////////////////////////////////////////////////////

  Declaration* ParseDeclaration();

  Declaration* ParsePrototype(bool require_sigature = false);
  FunDeclStatement* ParseFunPrototype(types::Type* hint);

  TraitDeclaration* ParseTraitDeclaration();
  TypeDeclStatement* ParseTypeDeclStatement();
  FunDeclStatement* ParseFunDeclStatement(types::Type* hint);
  VarDeclStatement* ParseVarDeclStatement(types::Type* hint);

  ////////////////////////////////////////////////////////////////////

  Expression* ParseExpression();

  Expression* ParseIfExpression();
  Expression* ParseNewExpression();
  Expression* ParseBlockExpression();

  Expression* ParseComparison();
  Expression* ParseBinary();

  Expression* ParseUnary();
  Expression* ParseDeref();
  Expression* ParseAddressof();

  // Precedence 1
  Expression* ParsePostfixExpressions();
  Expression* ParseFieldAccess(Expression* expr);
  Expression* ParseIndirectFieldAccess(Expression* expr);
  Expression* ParseIndexingExpression(Expression* expr);
  Expression* ParseFnCallUnnamed(Expression* expr);
  Expression* ParseFnCallExpression(Expression* expr, lex::Token id);

  Expression* ParseCompoundInitializer(lex::Token id);
  Expression* ParsePrimary();

  ////////////////////////////////////////////////////////////////////

  types::Type* ParseType();
  types::Type* ParseFunctionType();
  types::Type* ParsePointerType();
  types::Type* ParseStructType();
  types::Type* ParsePrimitiveType();

  ////////////////////////////////////////////////////////////////////

 private:
  auto ParseCSV() -> std::vector<Expression*>;
  auto ParseFormals() -> std::vector<lex::Token>;
  auto ParseDesignatedList()  //
      -> std::vector<CompoundInitializerExpr::Member>;

  bool Matches(lex::TokenType type);
  bool MatchesComparisonSign(lex::TokenType type);
  void Consume(lex::TokenType type);

  std::string FormatLocation();

 private:
  lex::Lexer& lexer_;
};
