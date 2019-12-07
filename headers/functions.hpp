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
 * Simple Functions take their parameters as template arguments
 *      to get the result (usually a Value), use ::Type
 * F Functions are higher order, they can be used as values.
 *      they take their parameters as template arguments to the ::Call member template
 *      to get the result, use ::Call<Args...>
 * the "call" helper allows for easier use of F Functions. It enables obtaining a result
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

/*
 * Select from two values based on condition
 */
class Select
{
    template <typename TakeFirst, typename TFirst, typename TSecond>
    struct SelectImpl;

    template <typename TakeFirst, typename TFirst, typename TSecond>
    using select = typename SelectImpl<TakeFirst, TFirst, TSecond>::Type;

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
    using Call = select<TakeFirst, TFirst, TSecond>;
};

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
        using Call = call<F, AllArgs...>;
    };

    template <typename F, typename... ArgsSoFar>
    struct CurryImpl<Bool<false>, F, ArgsSoFar...>
    {
        template <typename... ArgsToCome>
        using Call = CurryImpl<decltype(is_valid_call<F, ArgsSoFar..., ArgsToCome...>(nullptr)), F, ArgsSoFar..., ArgsToCome...>;
    };

public:
    template <typename F, typename... ArgsToCome>
    using Call = CurryImpl<decltype(is_valid_call<F, ArgsToCome...>(nullptr)), F, ArgsToCome...>;
};

/*
 * Test curried function, returns Bool<true> if passed int
 */
struct IsInt
{
    template <typename T>
    using Call = typename Curry::Call<Equals>::Call<int>::Call<T>::Call;
};

#endif
