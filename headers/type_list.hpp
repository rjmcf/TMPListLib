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


#endif
