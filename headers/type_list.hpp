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

#endif
