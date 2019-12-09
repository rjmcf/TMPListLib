#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

/*
 * Value semantics
 * To get the data out of a value, use ::Value
 */
template<typename T, T N>
struct Val
{
    static constexpr T Value{N};
};

/*
 * Bool Values
 * here, ::Value returns data of type bool
 */
template <bool B>
struct Bool : public Val<bool, B>
{};

/*
 * Function semantics
 * Functions are first order, they can be used as values.
 *      they take their parameters as template arguments to the ::Call member template
 *      to get the result, use ::Call<Args...>
 * the "call" helper allows for easier use of Functions. It enables obtaining a result
 *      by using call<F, Args...>
 */
 template<typename F, typename... Args>
 using call = typename F::template Call<Args...>;

/*
 * Test function, returns Bool<true> if passed the same type twice.
 */
class Equals
{
public:
    template<typename T, typename U>
    using Call = Bool<std::is_same<T, U>::value>;
};

template<typename T, typename U>
using equals = call<Equals, T, U>;

/*
 * Select from two values based on condition
 */
class Select
{
    template <typename TakeFirst, typename TFirst, typename TSecond>
    struct SelectImpl;

    template <typename TakeFirst, typename TFirst, typename TSecond>
    using selectImpl = typename SelectImpl<TakeFirst, TFirst, TSecond>::Type;

    template <typename TFirst, typename TSecond>
    struct SelectImpl<Bool<true>, TFirst, TSecond>
    {
        using Type = TFirst;
    };

    template <typename TFirst, typename TSecond>
    struct SelectImpl<Bool<false>, TFirst, TSecond>
    {
        using Type = TSecond;
    };

public:
    template <typename TakeFirst, typename TFirst, typename TSecond>
    using Call = selectImpl<TakeFirst, TFirst, TSecond>;
};

template <typename TakeFirst, typename TFirst, typename TSecond>
using select = call<Select, TakeFirst, TFirst, TSecond>;

/*
 * Curry
 * At the moment, requires a final Call after all arguments supplied
 */
class Curry
{
public:
    template <typename F, typename... Args, typename CheckIfValid = call<F, Args...>>
    static constexpr Bool<true> is_valid_call(std::nullptr_t);

    template <typename F, typename... Args>
    static constexpr Bool<false> is_valid_call(...);

private:
    template <typename IsValid, typename F, typename... ArgsSoFar>
    struct CurryImpl;

    template <typename F, typename... AllArgs>
    struct CurryImpl<Bool<true>, F, AllArgs...>
    {
        using Result = call<F, AllArgs...>;
    };

    template <typename F, typename... ArgsSoFar>
    struct CurryImpl<Bool<false>, F, ArgsSoFar...>
    {
        template <typename... ArgsToCome>
        using Call = CurryImpl<decltype(is_valid_call<F, ArgsSoFar..., ArgsToCome...>(nullptr)), F, ArgsSoFar..., ArgsToCome...>;
    };

public:
    template <typename F, typename... ArgsSoFar>
    using Call = CurryImpl<decltype(is_valid_call<F, ArgsSoFar...>(nullptr)), F, ArgsSoFar...>;
};

template <typename F, typename... ArgsSoFar>
using curry = call<Curry, F, ArgsSoFar...>;

/*
 * Int Values
 */
template <int I>
struct Int : public Val<int, I>
{};

/*
 * Test curried function, returns Bool<true> if passed Int<0>
 * Ideally should just be able to write
 * using IsZero = Curry::Call<Equals, Int<0>>
 * but can't do that due to the final Result required
 */
struct IsZero
{
    template <typename T>
    using Call = typename curry<Equals, Int<0>, T>::Result;
};

/*
 * Factorial
 */
class Factorial
{
    template <typename N, typename>
    struct FactImpl;
    template <typename N, typename Extra = void>
    using fact = typename FactImpl<N, Extra>::Type;

    template <int N, typename Extra>
    struct FactImpl<Int<N>, Extra>
    {
        using Type = Int<N * fact<Int<N-1>>::Value>;
    };

    template<typename Extra>
    struct FactImpl<Int<0>, Extra>
    {
        using Type = Int<1>;
    };

public:
    template <typename N>
    using Call = fact<N>;
};

#endif
