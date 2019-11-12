#include <type_traits>

#include "list_fns.hpp"
#include "list_fns_test.hpp"

template <template <typename FilterF, typename TList> class FilterImpl>
constexpr void test_filter()
{
    static_assert(std::is_same<FilterImpl<IsInt, void>, void>(),
        "Testing filter_impl applied to an empty list");
    static_assert(
        std::is_same<
            FilterImpl<IsInt, make_t<int, float, char, int, double>>,
            make_t<int, int>
        >(),
        "Testing applied to a longer list");
}

void test_list_fns()
{
    // Test that lists can store value
    static_assert(head< make_t<Val<int,45>> >::Value == 45,
        "Testing that Lists can store Vals");
    static_assert(head<tail< make_t<Val<int, 17>, Val<int, 42>> >>::Value == 42,
        "Testing that Lists can store Vals in their tails");

    // Test lists of functions
    static_assert(std::is_same<make_t<IsInt>, List<IsInt, void> >(),
        "Test that make_t applies to functions too");
    static_assert(call<head<make_t<IsInt>>, int>::Value,
        "Test that the head of a list of functions can be called");

    // Test is_list
    static_assert(!is_list<bool>::Value,
        "Testing that is_list returns false correctly");
    static_assert(is_list<void>::Value,
        "Testing that is_list returns true for an empty list");
    static_assert(is_list<List<bool, void>>::Value,
        "Testing that is_list returns true for a non-empty list");

    // Test append
    static_assert(std::is_same<append<int, void>, make_t<int>>(),
        "Testing append on empty list");
    static_assert(std::is_same<append<float, make_t<int>>, make_t<int, float>>(),
        "Testing append on small list");
    static_assert(
        std::is_same<
            append<float, make_t<char, int> >,
            make_t<char, int, float>
        >(),
        "Testing append on larger list");
    static_assert(std::is_same<AppendF::Call<int, void>, append<int, void>>(),
        "Testing that AppendF::Call works the same as append on an empty list");
    static_assert(std::is_same<AppendF::Call<int, make_t<float>>, append<int, make_t<float>> >(),
        "Testing that AppendF::Call works the same as append on a small list");

    // Test concat
    static_assert(std::is_same<concat<void, void>, void>(),
        "Testing concat on two empty lists");
    static_assert(std::is_same<concat<void, make_t<int>>, make_t<int> >(),
        "Testing concat when first list empty");
    static_assert(std::is_same<concat<make_t<int>, void>, make_t<int> >(),
        "Testing concat when second list empty");
    static_assert(std::is_same<concat<make_t<int>, make_t<float>>, make_t<int, float> >(),
        "Testing concat on two small lists");
    static_assert(
        std::is_same<
            concat< make_t<int, char>, make_t<float, double> >,
            make_t<int, char, float, double>
        >(),
        "Testing concat on two larger lists");
    static_assert(
        std::is_same<
            ConcatF::Call< make_t<int, char>, make_t<float, double> >,
            concat< make_t<int, char>, make_t<float, double> >
        >(),
        "Testing ConcatF::Call works the same as concat");

    // Test both filter impls
    test_filter<filter_mr>();
    test_filter<filter_sr>();
    // Test default filter, whatever it's been selected as
    test_filter<filter>();

    // Test map
    static_assert(std::is_same<map<IsInt, void>, void>(),
        "Testing map on empty list");
    static_assert(
        std::is_same<
            map<IsInt, make_t<int, char, float, int, double>>,
            make_t<Bool<true>, Bool<false>, Bool<false>, Bool<true>, Bool<false>>
        >(),
        "Testing map on IsInt and a larger list");

    // Test zip
    static_assert(std::is_same<zip<void,void>, void>(),
        "Testing zip on empty lists");
    static_assert(
        std::is_same<
            zip<make_t<int, float, char>, make_t<int, char, double>>,
            make_t<make_t<int,int>, make_t<float,char>, make_t<char,double>>
        >(),
        "Testing zip on larger lists");

    // Test flatten
    static_assert(std::is_same<flatten<void>, void>(),
        "Testing flatten on empty list");
    static_assert(std::is_same<flatten<make_t<make_t<>>>, void>(),
        "Testing flatten on a list containing an empty list");
    static_assert(std::is_same<flatten<make_t<make_t<make_t<>>>>, void>(),
        "Testing flatten on a list containing a list containing an empty list");
    static_assert(std::is_same<flatten<make_t<int, float>>, make_t<int, float>>(),
        "Testing flatten on a flat list");
    static_assert(
        std::is_same<
            flatten<make_t<make_t<int, float>, make_t<int, float>>>,
            make_t<int, float, int, float>
        >(),
        "Testing flatten on a 2D list");
    static_assert(
        std::is_same<
            flatten<make_t<make_t<int, float>, char, make_t<int, float>>>,
            make_t<int, float, char, int, float>
        >(),
        "Testing flatten on a mixed list");
    static_assert(
        std::is_same<
            flatten<make_t<make_t<make_t<int>, make_t<float, int>>, make_t<make_t<float, double>>>>,
            make_t<int, float, int, float, double>
        >(),
        "Testing flatten on a 3D list");
}
