#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

/*
 * Call function helper
 */
template<typename F, typename... Args>
using call = typename F::template Call<Args...>;

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

#endif
