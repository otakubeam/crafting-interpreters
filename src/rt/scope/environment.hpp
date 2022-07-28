#pragma once

#include <rt/base_object.hpp>

#include <unordered_map>
#include <optional>
#include <string>

class Environment {
 public:
  using Name = std::string;

  struct ScopeGuard {
    ScopeGuard(Environment** env) {
      auto new_env = new Environment{*env};
      *env = new_env;
    }

    ~ScopeGuard() {
      auto saved = *env;
      *env = (*env)->parent_scope_;
      delete saved;
    }

    Environment** env;
  };

  std::optional<SBObject> Get(Name name) {
    auto iter = state_.find(name);
    if (iter == state_.end()) {
      return parent_scope_ ?  //
                 (parent_scope_)->Get(std::move(name))
                           : std::nullopt;
    } else {
      return {iter->second};
    }
  }

  void InsertOrAssign(Name name, SBObject val) {
    state_.insert_or_assign(name, val);
  }

 private:
  Environment(Environment* parent) : parent_scope_{parent} {
  }

 private:
  Environment* parent_scope_;
  std::unordered_map<Name, SBObject> state_{};
};
