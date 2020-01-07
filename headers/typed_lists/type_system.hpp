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

// The type for integers
struct IntType : public Type
{};

// The type for bools
struct BoolType : public Type
{};

// The type for lists containing elements of a specific type
template <typename EType>
struct ListType : public Type
{
    using ElementType = EType;
};

// Storing a list of parameter types
template <typename TFirst, typename TOthers>
struct ParamList
{
    using First = TFirst;
    using Others = TOthers;
};

template <typename... TParams>
struct MakeP;
template <typename... TParams>
using make_p = typename MakeP<TParams...>::Ret;

template <typename TParam, typename... TParams>
struct MakeP<TParam, TParams...>
{
    using Ret = ParamList<TParam, make_p<TParams...>>;
};

template <>
struct MakeP<>
{
    using Ret = void;
};

// The type for functions
template <typename TReturn, typename... TParamTypes>
struct FunctionType : public Type
{
    using ReturnType = TReturn;
    using ParamTypes = make_p<TParamTypes...>;
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

// Used for checking if the supplied param types can convert to the required param types
template <typename ParamTypeList, typename... SuppliedParamTypes>
struct AllConvertible;
template <typename ParamTypeList, typename... SuppliedParamTypes>
constexpr bool all_convertible()
{
    return AllConvertible<ParamTypeList, SuppliedParamTypes...>::Ret;
}

template <typename FirstParam, typename OtherParams, typename FirstSupplied, typename... TOthersSupplied>
struct AllConvertible<ParamList<FirstParam, OtherParams>, FirstSupplied, TOthersSupplied...>
{
    static const bool Ret = is_convertible<FirstParam, FirstSupplied>() && all_convertible<OtherParams, TOthersSupplied...>();
};

template <>
struct AllConvertible<void>
{
    static const bool Ret = true;
};

// SFINAE implementation for checking that a parameter pack of argument types can convert to the list of
// parameter types
template <typename ParamTypeList, typename... SuppliedParamTypes>
using RequireMatches = std::enable_if_t<all_convertible<ParamTypeList, SuppliedParamTypes...>()>;


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

// @TODO decide whether function types can convert (and how)

#endif
