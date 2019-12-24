#ifndef TYPE_SYSTEM_H
#define TYPE_SYSTEM_H

#include <type_traits>

// Base class for all types
struct Type
{};

// A type that can represent any type
// @TODO work out way to distinguish AnyTypes, for instance the difference between
// 'a -> 'a list -> 'a list
// and
// 'b -> 'a list -> 'a list
struct AnyType : public Type
{};

// A type that represents ints
struct IntType : public Type
{};

// A type that represents bools
struct BoolType : public Type
{};

// A type that represents a list of a specific type
template <typename EType>
struct ListType : public Type
{
    using ElementType = EType;
};

// Used for deciding whether we can convert from one type to another
template <typename FromType, typename ToType>
struct Conversion
{
    static const bool Ret = false;
};

template <typename FromType, typename ToType>
constexpr bool is_convertible()
{
    return Conversion<FromType, ToType>::Ret;
}

template <typename FromType, typename ToType>
using RequireMatch = std::enable_if_t<is_convertible<FromType, ToType>()>;

template <typename FromType, typename ToType>
using convert_to = typename Conversion<FromType, ToType>::Type;

// The AnyType can convert to any type
template <typename T>
struct Conversion<AnyType, T>
{
    static const bool Ret = true;
    using Type = T;
};

// Specify that AnyType is convertible to itself
// @TODO this may not be correct implementation!
template <>
struct Conversion<AnyType, AnyType>
{
    static const bool Ret = true;
    using Type = AnyType;
};

// A type is always convertible to itself
template <typename T>
struct Conversion<T, T>
{
    static const bool Ret = true;
    using Type = T;
};

// Converting list types depends on converting their element types
template <typename FromType, typename ToType>
struct Conversion<ListType<FromType>, ListType<ToType>>
{
    static const bool Ret = is_convertible<FromType, ToType>();
    // @TODO only provide type if conversion succeeds
    using Type = ListType<ToType>;
};

// Specify that lists of the same type always convert
template <typename T>
struct Conversion<ListType<T>, ListType<T>>
{
    static const bool Ret = true;
    using Type = ListType<T>;
};

// Used for deciding whether each pair has a first that can be converted to the second
template <typename FirstType, typename SecondType>
struct Pair
{
    using First = FirstType;
    using Second = SecondType;
};

template <typename TPair>
using first = typename TPair::First;
template <typename TPair>
using second = typename TPair::Second;

template <typename... TPairs>
struct AllConvertible;
template <typename... TPairs>
constexpr bool all_convertible()
{
    return AllConvertible<TPairs...>::Ret;
}

template <typename FromType, typename ToType, typename... TPairs>
struct AllConvertible<Pair<FromType, ToType>, TPairs...>
{
    static const bool Ret = is_convertible<FromType, ToType>() && all_convertible<TPairs...>();
};

template <>
struct AllConvertible<>
{
    static const bool Ret = true;
};

template <typename... TPairs>
using RequireMatches = std::enable_if_t<all_convertible<TPairs...>()>;

#endif
