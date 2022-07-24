#pragma once

#include <ast/syntax_tree.hpp>
#include <ast/statements.hpp>

#include <lex/lexer.hpp>

#include <catch2/catch.hpp>

class Parser {
 public:
  Parser(lex::Lexer l) : lexer_{l} {
  }

  Expression* ParseExpression() {
    return ParseComparison();
  }

  Statement* ParseStatement() {
    if (auto if_stmt = ParseIfStatement()) {
      return if_stmt;
    }

    if (auto expr_stmt = ParseExprStatement()) {
      return expr_stmt;
    }

    // TODO: WHILE,VAR,BLOCK statemnt

    std::abort();
  }

  IfStatement* ParseIfStatement() {
    if (!Matches(lex::TokenType::IF)) {
      return nullptr;
    }

    // This should be fine even without parentheses, right?
    auto condition = ParseExpression();
    FMT_ASSERT(condition,  //
               "If statement without condition");

    auto true_branch = ParseStatement();
    FMT_ASSERT(true_branch,  //
               "If statement without true branch");

    Statement* false_branch = nullptr;

    if (Matches(lex::TokenType::ELSE)) {
      false_branch = ParseStatement();
      FMT_ASSERT(false_branch,  //
                 "Else close wihtout associated statemtn");
    }

    return new IfStatement(condition, true_branch, false_branch);
  }

  ExprStatement* ParseExprStatement() {
    auto expr = ParseExpression();
    Consume(lex::TokenType::SEMICOLUMN);

    return new ExprStatement{expr};
  }

  // clang-format off
  ///////////////////////////////////////////////////////////////////
  //                                                               //
  //   This area is used for literate prgramming                   //
  //                         ===================                   //
  //        Use it however you wish                                //
  //        -----------------------                                //
  //                                                               //
  //   The second big thing I need to do is to          (a)        //
  //   be able to parse statements.                                //
  //                    ----------                                 //
  //                                                               //
  //             - Specifically IF statement and VAR statement     //
  //                            ============     =============     //
  //                                                               //
  //    I can parse them now! Great. The only thing left is to     //
  //          ---------------        test the implementation.      //
  //                                                               //
  //    And of course I still need to fix visitors to recognize    //
  //     these new statements.     ---------------                 //
  //                                                               //
  //                                                               //
  //    It seems to be more useful to make 'if' an expression.     //
  //                               ============ --------------     //
  //    I wonder if there are any unintended consequences to that  //
  //                              -----------------------          //
  //                                                               //
  //                                                               //
  //                                                               //
  //                                                               //
  //                                                               //
  //                                                               //
  ///////////////////////////////////////////////////////////////////
  // clang-format on

  Expression* ParseComparison();
  Expression* ParseBinary();
  Expression* ParseUnary();
  Expression* ParsePrimary();

  ////////////////////////////////////////////////////////////////////

 private:
  bool Matches(lex::TokenType type) {
    if (lexer_.Peek().type != type) {
      return false;
    }

    lexer_.Advance();
    return true;
  }

  void Consume(lex::TokenType type) {
    auto error_msg = fmt::format("Could not match type {}",  //
                                 lex::FormatTokenType(type));
    FMT_ASSERT(Matches(type), error_msg.c_str());
  }

 private:
  lex::Lexer lexer_;
};
