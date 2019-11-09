#ifndef TYPE_LIST_H
#define TYPE_LIST_H

template <typename THead, typename TTail>
struct List
{
    using Head = THead;
    using Tail = TTail;
};

template <typename TList>
using head = typename TList::Head;

template <typename TList>
using tail = typename TList::Tail;

/*
 * Make a list from Variadic templates
 */
template <typename... Ts>
struct MakeT;
template <typename... Ts>
using make_t = typename MakeT<Ts...>::Value;

template <typename T, typename... Ts>
struct MakeT<T, Ts...>
{
    using Value = List<T, make_t<Ts...>>;
};

template<>
struct MakeT<>
{
    using Value = void;
};

/*
 * Append to a list
 */
template <typename T, typename TList>
struct Append;
template <typename T, typename TList>
using append = typename Append<T, TList>::Type;

template <typename T, typename THead, typename TTail>
struct Append<T, List<THead, TTail>>
{
    using Type = List<THead, append<T, TTail>>;
};

template <typename T>
struct Append<T, void>
{
    using Type = List<T, void>;
};

struct AppendF
{
    template <typename T, typename TList>
    using Call = append<T, TList>;
};

/*
 * Concatenate two lists
 */
template <typename TList1, typename TList2>
struct Concat;
template <typename TList1, typename TList2>
using concat = typename Concat<TList1, TList2>::Type;

template <typename THead, typename TTail, typename TList2>
struct Concat<List<THead, TTail>, TList2>
{
    using Type = List<THead, concat<TTail, TList2>>;
};

template <typename TList2>
struct Concat<void, TList2>
{
    using Type = TList2;
};

struct ConcatF
{
    template <typename TList1, typename TList2>
    using Call = concat<TList1, TList2>;
};

/*
 * Call function helper
 */
template<typename F, typename... Args>
using call = typename F::template Call<Args...>;

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

/*
 * Mutual Recursive Filter
 */
template <typename ShouldInclude, typename FilterF, typename TList>
struct FilterMR_Impl;
template <typename ShouldInclude, typename FilterF, typename TList>
using filter_mr_impl = typename FilterMR_Impl<ShouldInclude, FilterF, TList>::Type;

template <typename FilterF, typename TList>
struct FilterMR;
template <typename FilterF, typename TList>
using filter_mr = typename FilterMR<FilterF, TList>::Type;

template <typename FilterF, typename THead, typename TTail>
struct FilterMR_Impl<Bool<true>, FilterF, List<THead, TTail>>
{
    using Type = List<THead, filter_mr<FilterF, TTail>>;
};

template <typename FilterF, typename THead, typename TTail>
struct FilterMR_Impl<Bool<false>, FilterF, List<THead, TTail>>
{
    using Type = filter_mr<FilterF, TTail>;
};

template <typename FilterF, typename TList>
struct FilterMR
{
    using Type = filter_mr_impl<call<FilterF, head<TList>>, FilterF, TList>;
};

template <typename FilterF>
struct FilterMR<FilterF, void>
{
    using Type = void;
};

/*
 * Simply Recursive Filter
 */
template<typename FilterF, typename TList>
struct FilterSR;
template<typename FilterF, typename TList>
using filter_sr = typename FilterSR<FilterF, TList>::Type;

template<typename FilterF, typename THead, typename TTail>
struct FilterSR<FilterF, List<THead, TTail>>
{
    using Type = select_t<
/*If*/   call<FilterF,THead>,
/*Then*/ List<THead, filter_sr<FilterF,TTail>>,
/*Else*/ filter_sr<FilterF,TTail>
    >;
};

template<typename FilterF>
struct FilterSR<FilterF, void>
{
    using Type = void;
};

#endif
