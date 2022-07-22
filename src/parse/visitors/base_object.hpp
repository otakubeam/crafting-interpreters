#pragma once

#include <cstdint>
#include <variant>

//////////////////////////////////////////////////////////////////////

struct PrimitiveType {
  std::variant<        //
      std::nullptr_t,  //
      int,             //
      bool,            //
      char             //
      >
      ty_;
};

//////////////////////////////////////////////////////////////////////

inline PrimitiveType plus(PrimitiveType one, PrimitiveType two) {
  auto int_one = std::get<int>(one.ty_);
  auto int_two = std::get<int>(two.ty_);
  return {int_one + int_two};
}

inline PrimitiveType minus(PrimitiveType one, PrimitiveType two) {
  auto int_one = std::get<int>(one.ty_);
  auto int_two = std::get<int>(two.ty_);
  return {int_one - int_two};
}

//////////////////////////////////////////////////////////////////////

inline PrimitiveType bang(PrimitiveType one) {
  auto int_one = std::get<bool>(one.ty_);
  return {!int_one};
}

inline PrimitiveType negate(PrimitiveType one) {
  auto int_one = std::get<int>(one.ty_);
  return {-int_one};
}

//////////////////////////////////////////////////////////////////////

struct UserDefinedType {
  // TODO: Declaration object
  uint64_t* data = nullptr;
};

//////////////////////////////////////////////////////////////////////

using SBObject  =
  std::variant<         //
      PrimitiveType,    //
      UserDefinedType*  //
      >;

template<typename T>
inline T GetPrim(const SBObject& object) {
   auto prim = std::get<PrimitiveType>(object);
   return std::get<T>(prim);
}

template<typename T>
inline SBObject FromPrim(T value) {
   return SBObject{PrimitiveType{value}};
}

//////////////////////////////////////////////////////////////////////

// What do I allow for a type?

inline SBObject plus(SBObject one, SBObject two) {
  switch (one.index()) {
    case 0: {
      auto prim_one = std::get<PrimitiveType>(one);
      auto prim_two = std::get<PrimitiveType>(two);
      return {plus(prim_one, prim_two)};
    }
    default:
      throw "Cannot UDTs";
  }
}

inline SBObject minus(SBObject one, SBObject two) {
  switch (one.index()) {
    case 0: {
      auto prim_one = std::get<PrimitiveType>(one);
      auto prim_two = std::get<PrimitiveType>(two);
      return {minus(prim_one, prim_two)};
    }
    case 1:
    default:
      throw "Cannot UDTs";
  }
}

//////////////////////////////////////////////////////////////////////

inline SBObject bang(SBObject one) {
  switch (one.index()) {
    case 0:
      return {bang(std::get<0>(one))};
    case 1:
    default:
      throw "Cannot UDTs";
  }
}

inline SBObject negate(SBObject one) {
  switch (one.index()) {
    case 0:
      return {negate(std::get<0>(one))};
    case 1:
    default:
      throw "Cannot UDTs";
  }
}

//////////////////////////////////////////////////////////////////////
