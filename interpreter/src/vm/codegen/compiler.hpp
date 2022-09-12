#pragma once

#include <vm/codegen/detail/struct_symbol.hpp>
#include <vm/codegen/frame_translator.hpp>
#include <vm/chunk.hpp>

#include <ast/visitors/template_visitor.hpp>
#include <ast/scope/environment.hpp>

#include <ast/expressions.hpp>
#include <ast/statements.hpp>

#include <algorithm>
#include <ranges>

namespace vm::codegen {

class Compiler : public Visitor {
 public:
  virtual ~Compiler();

  ExecutableChunk Compile(TreeNode* node) {
    node->Accept(this);
    return chunk_;
  }

  static std::vector<ExecutableChunk>* CompileScript(TreeNode* node) {
    auto result = new std::vector<ExecutableChunk>;

    Compiler c;
    c.compiled_chunks_ = result;
    c.current = new FrameTranslator{};

    node->Accept(&c);

    result->push_back(c.chunk_);

    return result;
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitStatement(Statement* /* node */) override {
    FMT_ASSERT(false, "Visiting bare statement");
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitVarDecl(VarDeclStatement* node) override {
    // Generate code to place value on stack
    node->value_->Accept(this);

    // Infrom FrameTranslator about this location
    auto name = node->lvalue_->name_;
    current->AddLocal(name.GetName(), current->GetNextSize());
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitAssignment(AssignmentStatement* node) override {
    if (auto var = dynamic_cast<VarAccessExpression*>(node->target_)) {
      // Lookup this variable name
      auto mb_offset = current->Lookup(var->name_.GetName());
      auto offset = mb_offset.value();

      node->value_->Accept(this);

      chunk_.instructions.push_back(vm::Instr{
          .type = InstrType::STORE_STACK,
          .addr = (int16_t)offset,
      });

    } else {
      FMT_ASSERT(false, "Unimplemented!");
    }
  }

  virtual void VisitFunDecl(FunDeclStatement* node) override {
    FrameTranslator builder{node};

    Compiler chunk_compiler;
    chunk_compiler.current = &builder;
    // For the case of nested fn declarations
    chunk_compiler.compiled_chunks_ = compiled_chunks_;

    auto chunk = chunk_compiler.Compile(node->block_);

    chunk.instructions.push_back(vm::Instr{
        .type = InstrType::RET_FN,
    });

    int chunk_no = compiled_chunks_->size();
    uint8_t const_no = chunk_.attached_vals.size();

    chunk_.attached_vals.push_back(rt::PrimitiveValue{
        .tag = rt::ValueTag::Int,
        .as_int = chunk_no,
    });

    compiled_chunks_->push_back(chunk);

    // This allows for lookup of this symbol later at the callsite
    current->AddLocal(node->name_.GetName());

    chunk_.instructions.push_back(vm::Instr{
        .type = InstrType::PUSH_STACK,
        .arg1 = const_no,  // push the constant of the compiled chunk
    });
  }

  virtual void VisitFnCall(FnCallExpression* node) override {
    // Place in reverse order
    for (int i = node->arguments_.size() - 1; i >= 0; i -= 1) {
      node->arguments_[i]->Accept(this);
    }

    auto mb_offset = current->Lookup(node->fn_name_.GetName());

    int offset = mb_offset.value();

    // TODO: branch direct / indirect

    GenerateVarFetch(offset);

    chunk_.instructions.push_back(vm::Instr{
        .type = vm::InstrType::INDIRECT_CALL,
    });

    chunk_.instructions.push_back(vm::Instr{
        .type = vm::InstrType::FIN_CALL,
        .arg1 = (uint8_t)node->arguments_.size(),
    });
  }

 private:
  void GenerateVarFetch(int offset) {
    if (offset > 0) {
      chunk_.instructions.push_back(vm::Instr{
          .type = vm::InstrType::GET_LOCAL,
          .arg1 = (uint8_t)offset,
      });
    } else {
      chunk_.instructions.push_back(vm::Instr{
          .type = vm::InstrType::GET_ARG,
          .arg1 = (uint8_t)(-offset),
      });
    }
  }

 public:
  ////////////////////////////////////////////////////////////////////

  virtual void VisitStructDecl(StructDeclStatement* node) override {
    structs_.Declare(node->name_.GetName(),
                     new detail::StructSymbol{node, structs_});
  }

  ////////////////////////////////////////////////////////////////////

  struct ReturnedValue {
    // rt::SBObject value;
  };

  virtual void VisitReturn(ReturnStatement*) override {
    FMT_ASSERT(false, "Unimplemented!");
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitYield(YieldStatement*) override {
    FMT_ASSERT(false, "Unimplemented!");
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitExprStatement(ExprStatement* node) override {
    node->expr_->Accept(this);

    chunk_.instructions.push_back(vm::Instr{
        .type = InstrType::POP_STACK,
    });
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitExpression(Expression*) override {
    FMT_ASSERT(false, "Unreachable!");
  }

  virtual void VisitComparison(ComparisonExpression*) override {
    FMT_ASSERT(false, "Unimplemented!");
  }

  virtual void VisitBinary(BinaryExpression* node) override {
    node->left_->Accept(this);
    node->right_->Accept(this);

    switch (node->operator_.type) {
      case lex::TokenType::PLUS:
        chunk_.instructions.push_back(vm::Instr{
            .type = InstrType::ADD,
        });
        break;

      case lex::TokenType::MINUS:
        FMT_ASSERT(false, "Unimplemented!");

      default:
        FMT_ASSERT(false, "Unreachable!");
    }
  }

  virtual void VisitUnary(UnaryExpression*) override {
    FMT_ASSERT(false, "Unimplemented!");
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitIf(IfExpression* node) override {
    /////////

    node->condition_->Accept(this);

    auto jump_to_false_ip = chunk_.instructions.size();

    chunk_.instructions.push_back(vm::Instr{
        .type = InstrType::JUMP_IF_FALSE,
    });

    /////////

    node->true_branch_->Accept(this);

    auto jump_to_end_ip = chunk_.instructions.size();

    chunk_.instructions.push_back(vm::Instr{
        .type = InstrType::JUMP,
    });

    /////////

    auto false_ip_start = chunk_.instructions.size();

    node->false_branch_->Accept(this);

    auto false_ip_end = chunk_.instructions.size();

    // Backpatch JUMP_IF_FALSE
    chunk_.instructions.at(jump_to_false_ip) = vm::Instr{
        .type = InstrType::JUMP_IF_FALSE,
        .arg2 = (uint8_t)(false_ip_start >> 8),
        .arg3 = (uint8_t)(false_ip_start),
    };

    // Backpatch JUMP
    chunk_.instructions.at(jump_to_end_ip) = vm::Instr{
        .type = InstrType::JUMP,
        .arg2 = (uint8_t)(false_ip_end >> 8),
        .arg3 = (uint8_t)(false_ip_end),
    };
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitBlock(BlockExpression* node) override {
    for (auto& st : node->stmts_) {
      st->Accept(this);
    }

    if (node->final_) {
      node->final_->Accept(this);
      return;
    }

    AddNewConstant(999 /* mark */);
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitStructConstruction(
      StructConstructionExpression* node) override {
    auto fetch = structs_.Get(node->struct_name_.GetName());
    auto str_size = fetch.value()->Size();

    // Generate code for placing all initializers on stack
    for (auto v : node->values_) {
      v->Accept(this);
    }

    current->SetNextSize(str_size);
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitFieldAccess(FieldAccessExpression* node) override {
    node->GetAddress().
  }

  ////////////////////////////////////////////////////////////////////

  virtual void VisitLiteral(LiteralExpression* lit) override {
    chunk_.instructions.push_back(vm::Instr{
        .type = InstrType::PUSH_STACK,
        .arg1 = (uint8_t)chunk_.attached_vals.size(),
    });

    switch (lit->token_.type) {
      case lex::TokenType::NUMBER: {
        chunk_.attached_vals.push_back(vm::rt::PrimitiveValue{
            .tag = rt::ValueTag::Int,
            .as_int = std::get<int>(lit->token_.sem_info),
        });

        break;
      }

      case lex::TokenType::STRING:
        FMT_ASSERT(false, "Unimplemented!");

      case lex::TokenType::TRUE: {
        chunk_.attached_vals.push_back(vm::rt::PrimitiveValue{
            .tag = rt::ValueTag::Bool,
            .as_bool = true,
        });

        break;
      }

      case lex::TokenType::FALSE: {
        chunk_.attached_vals.push_back(vm::rt::PrimitiveValue{
            .tag = rt::ValueTag::Bool,
            .as_bool = false,
        });

        break;
      }

      default:
        FMT_ASSERT(false, "Unreachable!");
    }
  }

  virtual void VisitVarAccess(VarAccessExpression* node) override {
    auto mb_offset = current->Lookup(node->name_.GetName());
    int offset = mb_offset.value();
    GenerateVarFetch(offset);
  }

  ////////////////////////////////////////////////////////////////////

 private:
  void AddNewConstant(int value) {
    uint8_t const_no = chunk_.attached_vals.size();

    chunk_.attached_vals.push_back(rt::PrimitiveValue{
        .tag = rt::ValueTag::Int,
        .as_int = value,
    });

    chunk_.instructions.push_back(vm::Instr{
        .type = InstrType::PUSH_STACK,
        .arg1 = const_no,
    });
  }

 private:
  ExecutableChunk chunk_;

  std::vector<ExecutableChunk>* compiled_chunks_ = nullptr;

  // Environment
  using Env = Environment<detail::StructSymbol*>;
  Env structs_ = Env::MakeGlobal();

  // StackEmulation
  FrameTranslator* current = nullptr;
};

}  // namespace vm::codegen
