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
 * Test function, returns Bool<true> if passed int type.
 */
struct IsInt
{
    template<typename T>
    using Call = Bool<std::is_same<T, int>::value>;
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

#endif
