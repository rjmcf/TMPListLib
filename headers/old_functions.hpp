#ifndef OLD_FUNCTIONS_H
#define OLD_FUNCTIONS_H

#include "type_list.hpp"

/*
 * Values (copied from functions.hpp)
 */
 template<typename T, T N>
 struct Val
 {
     using Type = T;
     static const T Value{N};
 };

 /*
  * Bool Values
  * here, ::Value returns data of type bool
  */
 template <bool B>
 struct Bool : public Val<bool, B>
 {};

 /*
  * Int Values
  */
 template <int I>
 struct Int : public Val<int, I>
 {};

/*
 * Double function
 */
template <typename N>
struct Is0
{
    using Ret = Bool<false>;
};

template <>
struct Is0<Int<0>>
{
    using Ret = Bool<true>;
};

/*
 * Select
 */
template <typename TakeFirst, typename TFirst, typename TSecond>
struct Select;
template <typename TakeFirst, typename TFirst, typename TSecond>
using select = typename Select<TakeFirst, TFirst, TSecond>::Ret;

template <typename TFirst, typename TSecond>
struct Select<Bool<true>, TFirst, TSecond>
{
    using Ret = TFirst;
};

template <typename TFirst, typename TSecond>
struct Select<Bool<false>, TFirst, TSecond>
{
    using Ret = TSecond;
};

/*
 * call
 */
template <template<typename...> class F, typename... Args>
using call = typename F<Args...>::Ret;

/*
 * Filter Function
 */
template <template<typename...> class F, typename TList>
struct Filter;
template <template<typename...> class F, typename TList>
using filter = typename Filter<F, TList>::Ret;

template <template<typename...> class F, typename THead, typename TTail>
struct Filter<F, List<THead, TTail>>
{
private:
    using FilteredTail = filter<F, TTail>;
public:
    using Ret = select<call<F,THead>, List<THead, FilteredTail>, FilteredTail>;
};

template <template<typename...> class F>
struct Filter<F, void>
{
    using Ret = void;
};

/*
 * Function List
 */
template <template<typename...> class THead, typename TTail>
struct FList
{
    template<typename... Args>
    using Head = THead<Args...>;
    using Tail = TTail;
};

/*
 * Function taking function list
 */
template <template<template <typename...> class, typename> class THead, typename TTail>
struct FFList
{
    template<template <typename...> class F, typename TList>
    using Head = THead<F, TList>;
    using Tail = TTail;
};

#endif 
