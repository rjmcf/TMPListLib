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

// @TODO Test failures using new testing_utils

#endif
