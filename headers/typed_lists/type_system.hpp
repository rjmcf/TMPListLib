#ifndef TYPE_SYSTEM_H
#define TYPE_SYSTEM_H

#include <type_traits>

// Base class for all types
struct Type
{};

// A type that can represent any type
struct AnyType : public Type
{};

// A type that represents ints
struct IntType : public Type
{};

// A type that represents bools
struct BoolType : public Type
{};

// Used for deciding whether we can convert from one type to another
template <typename FromType, typename ToType>
struct IsConvertible
{
    static const bool Ret = false;
};

template <typename FromType, typename ToType>
constexpr bool is_convertible()
{
    return IsConvertible<FromType, ToType>::Ret;
}

template <typename FromType, typename ToType>
using convert_to = typename IsConvertible<FromType, ToType>::Type;

// The AnyType can convert to any type
template <typename T>
struct IsConvertible<AnyType, T>
{
    static const bool Ret = true;
    using Type = T;
};

// Specify that AnyType is convertible to itself
// @TODO this may not be correct implementation!
template <>
struct IsConvertible<AnyType, AnyType>
{
    static const bool Ret = true;
    using Type = AnyType;
};

// A type is always convertible to itself
template <typename T>
struct IsConvertible<T, T>
{
    static const bool Ret = true;
    using Type = T;
};

#endif
