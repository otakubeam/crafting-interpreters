#pragma once

#include <vm/rt/primitive.hpp>

#include <ast/statements.hpp>

#include <optional>
#include <vector>

namespace vm::codegen {

class FrameTranslator {
 public:
  struct Slot {
    std::string name;
    size_t depth = 0;
  };

  FrameTranslator(FunDeclStatement* decl) {
    for (auto fm : decl->formals_) {
      layout_.emplace_back(Slot{
          .name = "+formal." + fm.ident.GetName() + "@" +
                  std::to_string(current_depth_),
          .depth = 0,
      });
    }

    layout_.emplace_back(Slot{
        .name = "+ip@" + std::to_string(current_depth_),
        .depth = 0,
    });

    fp_ = layout_.size();

    layout_.emplace_back(Slot{
        .name = "+fp@" + std::to_string(current_depth_),
        .depth = 0,
    });
  }

  // Returns offset from the fp
  // (positive if in the current frame)
  // (nagative if from the past frames)
  std::optional<int> Lookup(std::string name) {
    // Check args
    for (size_t i = 0; i < fp_; i++) {
      auto& stack_name = layout_.at(i).name;
      if (stack_name == name) {
        return (i - fp_);
      }
    }

    // Check locals
    for (size_t i = fp_; i < layout_.size(); i++) {
      auto& stack_name = layout_.at(i).name;
      if (stack_name == name) {
        return i;
      }
    }

    return std::nullopt;
  }

  void AddLocal(std::string name) {
    layout_.emplace_back(Slot{
        .name = "+local." + name + "@" + std::to_string(current_depth_),
        .depth = current_depth_,
    });
  }

  void CreateNewScope() {
    current_depth_ += 1;
  }

  size_t TeardownInnermostScope() {
    size_t popped = 0;
    while (layout_.back().depth == current_depth_) {
      layout_.pop_back();
      popped += 1;
    }
    return popped;
  }

 private:
  std::vector<Slot> layout_;
  size_t current_depth_ = 0;
  size_t fp_ = 0;
};

}  // namespace vm::codegen
