#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <variant>
#include <string>

//////////////////////////////////////////////////////////////////////

using PrimitiveType = std::variant<  //
    std::nullptr_t,                  //
    int,                             //
    bool,                            //
    std::string                      //
    >;

//////////////////////////////////////////////////////////////////////

inline PrimitiveType BinaryOp(char op_type, PrimitiveType lhs,
                              PrimitiveType rhs) {
  auto int_one = std::get<int>(lhs);
  auto int_two = std::get<int>(rhs);
  return {op_type == '+' ? int_one + int_two : int_one - int_two};
}

inline PrimitiveType Plus(PrimitiveType one, PrimitiveType two) {
  return BinaryOp('+', one, two);
}

inline PrimitiveType Minus(PrimitiveType one, PrimitiveType two) {
  return BinaryOp('-', one, two);
}

//////////////////////////////////////////////////////////////////////

inline PrimitiveType Bang(PrimitiveType one) {
  auto int_one = std::get<bool>(one);
  return {!int_one};
}

inline PrimitiveType Negate(PrimitiveType one) {
  auto int_one = std::get<int>(one);
  return {-int_one};
}

//////////////////////////////////////////////////////////////////////

inline std::string Format(PrimitiveType value) {
  return std::visit(
      [](const auto& x) {
        return fmt::format("{}", x);
      },
      value);
}

//////////////////////////////////////////////////////////////////////
