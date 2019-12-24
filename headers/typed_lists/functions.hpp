#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "type_system.hpp"

/*
 * Value semantics
 */
template <typename TType>
struct Val
{
    using Type = TType;
};

template <typename TVal>
using TypeOf = typename TVal::Type;

/*
 * Int values
 */
template <int I>
struct Int : Val<IntType>
{
    static const int Value = I;
};

/*
 * Bool values
 */
template <bool B>
struct Bool : Val<BoolType>
{
    static const bool Value = B;
};

/*
 * Not function
 */
struct Not
{
    using InputType = BoolType;

private:
    template <typename TIn, typename Enable = void>
    struct NotImpl;

    template <typename TIn>
    using notImpl = typename NotImpl<TIn>::Ret;

    // This definition limits the Not function to only taking BoolType inputs,
    // or things that BoolType can convert to. Thus, even though the int value
    // of an IntType would be converted to bool and this could compile, it won't
    template <typename TIn>
    struct NotImpl<TIn, RequireMatch<InputType, TypeOf<TIn>>>
    {
        using Ret = Bool<!TIn::Value>;
    };

public:
    template <typename TIn>
    using Call = notImpl<TIn>;
};

/*
 * And Function
 */
struct And
{
    using InputType = BoolType;

private:
    template <typename TFirst, typename TSecond, typename Enable = void>
    struct AndImpl;

    template <typename TFirst, typename TSecond>
    using andImpl = typename AndImpl<TFirst, TSecond>::Ret;

    template <typename TFirst, typename TSecond>
    struct AndImpl<TFirst, TSecond, RequireMatches<
        Pair<InputType, TypeOf<TFirst>>,
        Pair<InputType, TypeOf<TSecond>>
    >>
    {
        using Ret = Bool<TFirst::Value && TSecond::Value>;
    };

public:
    template <typename TFirst, typename TSecond>
    using Call = andImpl<TFirst, TSecond>;
};

#endif