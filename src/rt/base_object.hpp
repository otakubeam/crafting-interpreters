#pragma once

#include <rt/user_defined_type.hpp>
#include <rt/primitive_type.hpp>
#include <rt/type_error.hpp>

//////////////////////////////////////////////////////////////////////

using SBObject = std::variant<  //
    PrimitiveType,              //
    UserDefinedType*            //
    >;

//////////////////////////////////////////////////////////////////////

template <typename T>
inline T GetPrim(const SBObject& object) {
  auto prim = std::get<PrimitiveType>(object);
  return std::get<T>(prim);
}

template <typename T>
inline SBObject FromPrim(T value) {
  return SBObject{PrimitiveType{value}};
}

//////////////////////////////////////////////////////////////////////

inline SBObject BinaryOp(char op_type, SBObject lhs, SBObject rhs) {
  try {
    auto prim_one = std::get<PrimitiveType>(lhs);
    auto prim_two = std::get<PrimitiveType>(rhs);
    return {BinaryOp(op_type, prim_one, prim_two)};
  } catch (std::bad_variant_access&) {
    throw TypeError{};
  }
}

inline SBObject plus(SBObject one, SBObject two) {
  return BinaryOp('+', one, two);
}

inline SBObject minus(SBObject one, SBObject two) {
  return BinaryOp('-', one, two);
}

//////////////////////////////////////////////////////////////////////

inline SBObject UnaryOp(char op_type, SBObject operand) {
  try {
    PrimitiveType prim_operand = std::get<PrimitiveType>(operand);
    return {op_type == '!' ? Bang(prim_operand)  //
                           : Negate(prim_operand)};
  } catch (std::bad_variant_access&) {
    throw TypeError{};
  }
}

inline SBObject bang(SBObject one) {
  return UnaryOp('!', one);
}

inline SBObject negate(SBObject one) {
  return UnaryOp('-', one);
}

//////////////////////////////////////////////////////////////////////

inline std::string Format(SBObject object) {
  try {
    PrimitiveType prim_operand = std::get<PrimitiveType>(object);
    return Format(prim_operand);
  } catch (std::bad_variant_access&) {
    throw TypeError{};
  }
}

//////////////////////////////////////////////////////////////////////