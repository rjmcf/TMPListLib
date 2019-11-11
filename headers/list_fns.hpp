#ifndef LIST_FNS_HPP
#define LIST_FNS_HPP

#include "type_list.hpp"
#include "functions.hpp"

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

struct FilterMR_F
{
    template <typename FilterF, typename TList>
    using Call = filter_mr<FilterF, TList>;
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

struct FilterSR_F
{
    template<typename FilterF, typename TList>
    using Call = filter_sr<FilterF, TList>;
};

/*
 * Choosing default Filter
 */
template<typename FilterF, typename TList>
using Filter = FilterSR<FilterF, TList>;
template<typename FilterF, typename TList>
using filter = typename Filter<FilterF, TList>::Type;

struct FilterF
{
    template<typename FilterF, typename TList>
    using Call = filter<FilterF, TList>;
};

/*
 * Map function
 */
template <typename F, typename TList>
struct Map;
template <typename F, typename TList>
using map = typename Map<F, TList>::Type;

template <typename F, typename THead, typename TTail>
struct Map<F, List<THead, TTail>>
{
    using Type = List<call<F, THead>, map<F, TTail>>;
};

template <typename F>
struct Map<F, void>
{
    using Type = void;
};

struct MapF
{
    template <typename F, typename TList>
    using Call = map<F, TList>;
};

/*
 * Zip function
 */
template <typename TList1, typename TList2>
struct Zip;
template <typename TList1, typename TList2>
using zip = typename Zip<TList1, TList2>::Type;

template <typename THead1, typename TTail1, typename THead2, typename TTail2>
struct Zip<List<THead1, TTail1>, List<THead2, TTail2>>
{
    using Type = List<make_t<THead1, THead2>, zip<TTail1, TTail2>>;
};

template<>
struct Zip<void, void>
{
    using Type = void;
};

struct ZipF
{
    template <typename TList1, typename TList2>
    using Call = zip<TList1, TList2>;
};

/*
 * IsList function
 */
template <typename T>
struct IsList
{
    using Type = Bool<false>;
};
template <typename T>
using is_list = typename IsList<T>::Type;

template <typename THead, typename TTail>
struct IsList<List<THead, TTail>>
{
    using Type = Bool<true>;
};

template <>
struct IsList<void>
{
    using Type = Bool<true>;
};

struct IsListF
{
    template <typename T>
    using Call = is_list<T>;
};

/*
 * Flatten function
 */
template <typename TList>
struct Flatten;
template <typename TList>
using flatten = typename Flatten<TList>::Type;

template<typename IsList, typename TList>
struct FlattenImpl;
template<typename IsList, typename TList>
using flatten_impl = typename FlattenImpl<IsList, TList>::Type;

template<typename THead, typename TTail>
struct FlattenImpl<Bool<true>, List<THead, TTail>>
{
    using Type = concat<flatten<THead>, flatten<TTail>>;
};

template<typename THead, typename TTail>
struct FlattenImpl<Bool<false>, List<THead, TTail>>
{
    using Type = List<THead, flatten<TTail>>;
};

template<typename THead, typename TTail>
struct Flatten< List<THead, TTail> >
{
    using Type = flatten_impl<is_list<THead>, List<THead, TTail>>;
};

template<>
struct Flatten<void>
{
    using Type = void;
};

struct FlattenF
{
    template <typename TList>
    using Call = call<TList>;
};

#endif
