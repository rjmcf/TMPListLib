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
template<typename T, typename U>
struct Equals
{
    using Type = Bool<std::is_same<T, U>::value>;
};
template<typename T, typename U>
using equals = typename Equals<T, U>::Type;

struct EqualsF
{
    template<typename T, typename U>
    using Call = equals<T, U>;
};

/*
 * Select from two values based on condition
 */
template <typename TakeFirst, typename TFirst, typename TSecond>
struct Select;
template <typename TakeFirst, typename TFirst, typename TSecond>
using select_t = typename Select<TakeFirst, TFirst, TSecond>::Type;

template <typename TFirst, typename TSecond>
struct Select<Bool<true>, TFirst, TSecond>
{
    using Type = TFirst;
};

template <typename TFirst, typename TSecond>
struct Select<Bool<false>, TFirst, TSecond>
{
    using Type = TSecond;
};

struct SelectF
{
    template <typename TakeFirst, typename TFirst, typename TSecond>
    using Call = select_t<TakeFirst, TFirst, TSecond>;
};

/*
 * Currying a function
 * This doesn't meld well with the current function semantics.
 * Given a curried function CF, implicitly an F Function, call<CF, Args...>
 * returns another F Function when Args... does not contain all the required arguments,
 * and a Simple Function when it does. This is inconsistent with normal F Functions,
 * since a curried function called with "call" requires an extra ::Type to get the value
 * Because we need to use sfinae to select the correct result when the function call is valid,
 * I don't think we can get rid of this extra Type call. Therefor instead I should
 * make *all* F Functions require the extra Type, after Call. Then all function usage
 * is consistent.
 */
template <typename F, typename... Args, typename CheckIfValid = call<F, Args...>>
constexpr Bool<true> is_valid_call(std::nullptr_t);

template <typename F, typename... Args>
constexpr Bool<false> is_valid_call(...);

template <typename IsValid, typename F, typename... ArgsSoFar>
struct CurryImpl;

template <typename F, typename... AllArgs>
struct CurryImpl<Bool<true>, F, AllArgs...>
{
    using Type = call<F, AllArgs...>;
};

template <typename F, typename... ArgsSoFar>
struct CurryImpl<Bool<false>, F, ArgsSoFar...>
{
    template <typename... ArgsToCome>
    using Call = CurryImpl<decltype(is_valid_call<F, ArgsSoFar..., ArgsToCome...>(nullptr)), F, ArgsSoFar..., ArgsToCome...>;
};

template <typename F>
struct Curry
{
    template <typename... ArgsToCome>
    using Call = CurryImpl<decltype(is_valid_call<F, ArgsToCome...>(nullptr)), F, ArgsToCome...>;
};

/*
 * Test curried function, returns Bool<true> if passed int
 */
template <typename T>
struct IsInt
{
    using Type = typename Curry<EqualsF>::Call<int>::Call<T>::Type;
};

struct IsIntF
{
    template <typename T>
    using Call = typename IsInt<T>::Type;
};

#endif
