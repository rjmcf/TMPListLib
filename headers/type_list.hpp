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

#endif
