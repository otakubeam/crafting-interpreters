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
 private:
  // Invoked by VmMemory
  VmStack(uint8_t* memory) : stack_area_{(rt::PrimitiveValue*)memory} {
  }

 public:
  friend class debug::StackPrinter;
  friend class memory::VmMemory;

  void Push(rt::PrimitiveValue value) {
    stack_area_[sp_] = std::move(value);
    sp_ += 1;
  }

  auto Pop() -> rt::PrimitiveValue {
    sp_ -= 1;
    return stack_area_[sp_];
  }

  void PopCount(size_t count) {
    sp_ -= count;
  }

  void Ret() {
    // Move the stack pointer
    sp_ = fp_;

    // Expect by ABI to be a StackRef
    FMT_ASSERT(stack_area_[fp_].tag == rt::ValueTag::StackRef,
               "Expected an StackRef in fp slot");

    // Move the frame pointer
    fp_ = stack_area_[fp_].as_ref.to_data;
  }

  void PrepareCallframe() {
    // Save curent fp in sp
    stack_area_[sp_] = rt::PrimitiveValue{
        .tag = rt::ValueTag::StackRef,
        .as_ref = rt::Reference{.to_data = (uint32_t)fp_},
    };

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

  auto GetSavedIp() -> rt::InstrReference {
    return GetAtFp(-1).as_ref.to_instr;
  }

  auto Top() -> rt::PrimitiveValue& {
    return stack_area_[sp_ - 1];
  }

 private:
  auto GetAtFp(int offset) -> rt::PrimitiveValue& {
    return stack_area_[fp_ + offset];
  }

 private:
  // Stack pointer, frame pointer
  size_t sp_ = 1;
  size_t fp_ = 0;

  rt::PrimitiveValue* stack_area_ = nullptr;
};

}  // namespace vm
