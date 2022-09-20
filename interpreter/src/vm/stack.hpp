#pragma once

#include <vm/rt/primitive.hpp>

#include <cstdlib>
#include <vector>

// Forward declare class from another namespace
namespace vm::debug {
class StackPrinter;
}

namespace vm::memory {
class VmMemory;
}

namespace vm {

class VmStack {
 public:
  friend class debug::StackPrinter;
  friend class memory::VmMemory;

  void Push(rt::PrimitiveValue value) {
    stack_.at(sp_) = std::move(value);
    sp_ += 1;
  }

  auto Pop() -> rt::PrimitiveValue {
    sp_ -= 1;
    return stack_.at(sp_);
  }

  void PopCount(size_t count) {
    sp_ -= count;
  }

  void Ret() {
    // Move the stack pointer
    sp_ = fp_;

    // Expect by ABI to be an integer offset
    FMT_ASSERT(stack_.at(fp_).tag == rt::ValueTag::Int,
               "Expected an Int in fp slot");

    // Move the frame pointer
    fp_ = stack_.at(fp_).as_int;
  }

  void PrepareCallframe() {
    // Save curent fp in sp
    stack_.at(sp_) = rt::PrimitiveValue{.tag = rt::ValueTag::Int,  //
                                        .as_int = (int)fp_};

    // Move fp to sp
    fp_ = sp_;

    // Point to the next empty slot
    sp_ += 1;
  }

  auto GetFnArg(size_t count) -> rt::PrimitiveValue {
    return GetAtFp(-count);
  }

  auto GetLocalVar(size_t count) -> rt::PrimitiveValue {
    return GetAtFp(count);
  }

  void StoreAt(int16_t offset, rt::PrimitiveValue value) {
    GetAtFp(offset) = value;
  }

  int GetSavedIp() {
    return GetAtFp(-1).as_int;
  }

  auto Top() -> rt::PrimitiveValue& {
    return stack_.at(sp_ - 1);
  }

  auto Bottom() -> rt::PrimitiveValue* {
    return &stack_.at(0);
  }

 private:
  auto GetAtFp(int offset) -> rt::PrimitiveValue& {
    return stack_.at(fp_ + offset);
  }

 private:
  // Stack pointer, frame pointer
  size_t sp_ = 1;
  size_t fp_ = 0;

  std::vector<rt::PrimitiveValue> stack_{10000000};
};

}  // namespace vm
