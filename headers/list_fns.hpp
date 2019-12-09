#ifndef LIST_FNS_HPP
#define LIST_FNS_HPP

#include "type_list.hpp"
#include "functions.hpp"

/*
 * IsList function
 */
class IsList
{
    template <typename T, typename>
    struct IsListImpl
    {
        using Type = Bool<false>;
    };

    template <typename T, typename Extra = void>
    using is_listImpl = typename IsListImpl<T, Extra>::Type;

    template <typename THead, typename TTail, typename Extra>
    struct IsListImpl<List<THead, TTail>, Extra>
    {
        using Type = Bool<true>;
    };

    template <typename Extra>
    struct IsListImpl<void, Extra>
    {
        using Type = Bool<true>;
    };

public:
    template <typename T>
    using Call = is_listImpl<T>;
};

template <typename T>
using is_list = call<IsList, T>;

/*
 * Length
 */
class Length
{
    template <typename TList, typename>
    struct LengthImpl;
    template <typename TList, typename Extra = void>
    using lengthImpl = typename LengthImpl<TList, Extra>::Type;

    template <typename THead, typename TTail, typename Extra>
    struct LengthImpl<List<THead, TTail>, Extra>
    {
        using Type = Int<1 + lengthImpl<TTail>::Value>;
    };

    template<typename Extra>
    struct LengthImpl<void, Extra>
    {
        using Type = Int<0>;
    };

public:
    template <typename TList>
    using Call = lengthImpl<TList>;
};

template <typename TList>
using length = call<Length, TList>;

/*
 * Fill, used to generate a list of N of the same element
 */
class Fill
{
    template <typename N, typename T>
    struct FillImpl;
    template <typename N, typename T>
    using fillImpl = typename FillImpl<N,T>::Type;

    template <int N, typename T>
    struct FillImpl<Int<N>, T>
    {
        using Type = List<T, fillImpl<Int<N-1>,T>>;
    };

    template <typename T>
    struct FillImpl<Int<0>, T>
    {
        using Type = void;
    };

public:
    template <typename N, typename T>
    using Call = fillImpl<N, T>;
};

template <typename N, typename T>
using fill = call<Fill, N, T>;

/*
 * Append to a list
 */
class Append
{
    template <typename T, typename TList>
    struct AppendImpl;

    template <typename T, typename TList>
    using append = typename AppendImpl<T,TList>::Type;

    template <typename T, typename THead, typename TTail>
    struct AppendImpl<T, List<THead, TTail>>
    {
        using Type = List<THead, typename AppendImpl<T, TTail>::Type>;
    };

    template <typename T>
    struct AppendImpl<T, void>
    {
        using Type = List<T, void>;
    };

public:
    template <typename T, typename TList>
    using Call = append<T, TList>;
};

/*
 * Concatenate two lists
 */
class Concat
{
    template <typename TList1, typename TList2>
    struct ConcatImpl;

    template <typename TList1, typename TList2>
    using concat = typename ConcatImpl<TList1, TList2>::Type;

    template <typename THead, typename TTail, typename TList2>
    struct ConcatImpl<List<THead, TTail>, TList2>
    {
        using Type = List<THead, typename ConcatImpl<TTail, TList2>::Type>;
    };

    template <typename TList2>
    struct ConcatImpl<void, TList2>
    {
        using Type = TList2;
    };

public:
    template <typename TList1, typename TList2>
    using Call = concat<TList1, TList2>;
};

template <typename TList1, typename TList2>
using concat = call<Concat, TList1, TList2>;

/*
 * Filter
 */
class Filter
{
    template<typename FilterF, typename TList>
    struct FilterImpl;
    template<typename FilterF, typename TList>
    using filter = typename FilterImpl<FilterF, TList>::Type;

    template<typename FilterF, typename THead, typename TTail>
    struct FilterImpl<FilterF, List<THead, TTail>>
    {
        using Type = select<
    /*If*/   call<FilterF,THead>,
    /*Then*/ List<THead, filter<FilterF,TTail>>,
    /*Else*/ filter<FilterF,TTail>
        >;
    };

    template<typename FilterF>
    struct FilterImpl<FilterF, void>
    {
        using Type = void;
    };

public:
    template<typename FilterF, typename TList>
    using Call = filter<FilterF, TList>;
};

/*
 * Map function
 */
class Map
{
    template <typename F, typename TList>
    struct MapImpl;
    template <typename F, typename TList>
    using map = typename MapImpl<F, TList>::Type;

    template <typename F, typename THead, typename TTail>
    struct MapImpl<F, List<THead, TTail>>
    {
        using Type = List<call<F, THead>, map<F, TTail>>;
    };

    template <typename F>
    struct MapImpl<F, void>
    {
        using Type = void;
    };

public:
    template <typename F, typename TList>
    using Call = map<F, TList>;
};

template <typename F, typename TList>
using map = call<Map, F, TList>;

/*
 * Zip function
 */
class Zip
{
    template <typename TList1, typename TList2, typename>
    struct ZipImpl;
    template <typename TList1, typename TList2, typename Extra = void>
    using zip = typename ZipImpl<TList1, TList2, Extra>::Type;

    template <typename THead1, typename TTail1, typename THead2, typename TTail2, typename Extra>
    struct ZipImpl<List<THead1, TTail1>, List<THead2, TTail2>, Extra>
    {
        using Type = List<make_t<THead1, THead2>, zip<TTail1, TTail2>>;
    };

    template<typename Extra>
    struct ZipImpl<void, void, Extra>
    {
        using Type = void;
    };

public:
    template <typename TList1, typename TList2>
    using Call = zip<TList1, TList2>;
};

/*
 * Flatten function
 */
class Flatten
{
    template <typename TList, typename>
    struct FlattenImpl;
    template <typename TList, typename Extra = void>
    using flatten = typename FlattenImpl<TList, Extra>::Type;

    // Need separate FlattenSelect partial specs, since
    // some types would be invalid if written as single
    // Select statement
    template<typename IsList, typename TList>
    struct FlattenSelect;
    template<typename IsList, typename TList>
    using flatten_s = typename FlattenSelect<IsList, TList>::Type;

    template<typename THead, typename TTail>
    struct FlattenSelect<Bool<true>, List<THead, TTail>>
    {
        using Type = concat<flatten<THead>, flatten<TTail>>;
    };

    template<typename THead, typename TTail>
    struct FlattenSelect<Bool<false>, List<THead, TTail>>
    {
        using Type = List<THead, flatten<TTail>>;
    };

    template<typename THead, typename TTail, typename Extra>
    struct FlattenImpl< List<THead, TTail>, Extra >
    {
        using Type = flatten_s<is_list<THead>, List<THead, TTail>>;
    };

    template<typename Extra>
    struct FlattenImpl<void, Extra>
    {
        using Type = void;
    };

public:
    template <typename TList>
    using Call = flatten<TList>;
};

/*
 * Zip Apply.
 * Takes a list of single arg functions and a list of arguments
 * returns a list where the ith element is the ith function
 * applied to the ith argument
 */
class ZipApply
{
    template <typename Fs, typename Args, typename>
    struct ZipAppImpl;
    template <typename Fs, typename Args, typename Extra = void>
    using zip_app = typename ZipAppImpl<Fs, Args, Extra>::Type;

    template <typename FHead, typename FTail, typename AHead, typename ATail, typename Extra>
    struct ZipAppImpl<List<FHead, FTail>, List<AHead, ATail>, Extra>
    {
        using Type = List<call<FHead, AHead>, zip_app<FTail, ATail>>;
    };

    template <typename Extra>
    struct ZipAppImpl<void, void, Extra>
    {
        using Type = void;
    };

public:
    template <typename Fs, typename Args>
    using Call = zip_app<Fs, Args>;
};

template <typename Fs, typename Args>
using zip_apply = call<ZipApply, Fs, Args>;


/*
 * MapN
 * Takes a function of N arguments, and N lists of length M
 * Produces a single list of length M whose ith element is the
 * result of applying the function to all the ith elements of
 * the N lists.
 */
class MapN
{
    template <typename F, typename Acc, typename... TLists>
    struct MapNImpl;
    template <typename F, typename Acc, typename... TLists>
    using mapN = typename MapNImpl<F, Acc, TLists...>::Type;

    template <typename F, typename Acc, typename TFirst, typename... TRest>
    struct MapNImpl<F, Acc, TFirst, TRest...>
    {
        using Type = mapN<F, zip_apply<Acc, TFirst>, TRest...>;
    };

    struct FinalApply
    {
        template <typename F>
        using Call = typename F::Result;
    };

    template <typename F, typename Acc>
    struct MapNImpl<F, Acc>
    {
        using Type = map<FinalApply, Acc>;
    };

    // If given empty lists
    template <typename F, typename... Empties>
    struct MapNImpl<F, void, void, Empties...>
    {
        using Type = void;
    };

    template <typename F, typename TFirst, typename... TRest>
    struct MapNImpl<F, void, TFirst, TRest...>
    {
        using Type = mapN<F, fill<length<TFirst>, curry<F>>, TFirst, TRest...>;
    };

public:
    template <typename F, typename... TLists>
    using Call = mapN<F, void, TLists...>;
};

#endif
