#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

/*
 * Value Semantics
 */
template<typename T, T N>
struct Val
{
    static constexpr T Value{N};
};

/*
 * Bool Values
 */
template <bool B>
struct Bool : public Val<bool, B>
{};

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
 * Call function helper
 */
template<typename F, typename... Args>
using call = typename F::template Call<Args...>;

/*
 * Currying a function
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
