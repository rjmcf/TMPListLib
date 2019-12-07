#include <type_traits>

#include "list_fns.hpp"
#include "list_fns_test.hpp"

void test_list_fns()
{
    // Test that lists can store value
    static_assert(head< make_t<Val<int,45>> >::Value == 45,
        "Testing that Lists can store Vals");
    static_assert(head<tail< make_t<Val<int, 17>, Val<int, 42>> >>::Value == 42,
        "Testing that Lists can store Vals in their tails");

    // Test lists of functions
    static_assert(std::is_same<make_t<IsZero>, List<IsZero, void> >(),
        "Test that make_t applies to functions too");
    static_assert(call<head<make_t<IsZero>>, Int<0>>::Value,
        "Test that the head of a list of functions can be called");
    static_assert(std::is_same<make_t<Curry>, List<Curry, void> >(),
        "Test that make_t applies to the Curry function");

    // Test list of curried functions
    static_assert(std::is_same<make_t<Curry::Call<Equals>>, List<Curry::Call<Equals>, void> >(),
        "Test that make_t applies to curried functions too");
    static_assert(
        std::is_same<
            call< head< make_t<Curry::Call<Equals>> >, int>,
            call< Curry::Call<Equals>, int>
        >(),
        "Test that the head of a list of curried functions can be called");
    static_assert(
        std::is_same<
            call< call< head< make_t<Curry::Call<Equals>> >, int>, int>,
            call< call< Curry::Call<Equals>, int>, int>
        >(),
        "Test that the head of a list of curried functions can be called mutliple times");
    static_assert(call< call< head< make_t<Curry::Call<Equals>> >, int>, int>::Call::Value,
        "Test that the head of a list of curried functions can be called mutliple times and return a value");

    // Test is_list
    static_assert(!IsList::Call<bool>::Value,
        "Testing that is_list returns false correctly");
    static_assert(IsList::Call<void>::Value,
        "Testing that is_list returns true for an empty list");
    static_assert(IsList::Call<List<bool, void>>::Value,
        "Testing that is_list returns true for a non-empty list");

    // Test length
    static_assert(Length::Call<void>::Value == 0,
        "Testing length for an empty list");
    static_assert(Length::Call<make_t<int>>::Value == 1,
        "Testing length for a single item list");
    static_assert(Length::Call<make_t<int, float, double>>::Value == 3,
        "Testing length for a longer list");

    // Test fill
    static_assert(std::is_same<Fill::Call<Int<0>, int>, void>(),
        "Testing filling an empty list");
    static_assert(std::is_same<Fill::Call<Int<3>, int>, make_t<int,int,int>>(),
        "Testing filling a list");

    // Test append
    static_assert(std::is_same<Append::Call<int, void>, make_t<int>>(),
        "Testing append on empty list");
    static_assert(std::is_same<Append::Call<float, make_t<int>>, make_t<int, float>>(),
        "Testing append on small list");
    static_assert(
        std::is_same<
            Append::Call<float, make_t<char, int> >,
            make_t<char, int, float>
        >(),
        "Testing append on larger list");

    // Test concat
    static_assert(std::is_same<Concat::Call<void, void>, void>(),
        "Testing concat on two empty lists");
    static_assert(std::is_same<Concat::Call<void, make_t<int>>, make_t<int> >(),
        "Testing concat when first list empty");
    static_assert(std::is_same<Concat::Call<make_t<int>, void>, make_t<int> >(),
        "Testing concat when second list empty");
    static_assert(std::is_same<Concat::Call<make_t<int>, make_t<float>>, make_t<int, float> >(),
        "Testing concat on two small lists");
    static_assert(
        std::is_same<
            Concat::Call< make_t<int, char>, make_t<float, double> >,
            make_t<int, char, float, double>
        >(),
        "Testing concat on two larger lists");

    // Test filter
    static_assert(std::is_same<Filter::Call<IsZero, void>, void>(),
        "Testing filter_impl applied to an empty list");
    static_assert(
        std::is_same<
            Filter::Call<IsZero, make_t<Int<0>, float, char, Int<0>, double>>,
            make_t<Int<0>, Int<0>>
        >(),
        "Testing applied to a longer list");

    // Test map
    static_assert(std::is_same<Map::Call<IsZero, void>, void>(),
        "Testing map on empty list");
    static_assert(
        std::is_same<
            Map::Call<IsZero, make_t<Int<0>, char, float, Int<0>, double>>,
            make_t<Bool<true>, Bool<false>, Bool<false>, Bool<true>, Bool<false>>
        >(),
        "Testing map on IsInt and a larger list");

    // Test zip
    static_assert(std::is_same<Zip::Call<void,void>, void>(),
        "Testing zip on empty lists");
    static_assert(
        std::is_same<
            Zip::Call<make_t<int, float, char>, make_t<int, char, double>>,
            make_t<make_t<int,int>, make_t<float,char>, make_t<char,double>>
        >(),
        "Testing zip on larger lists");

    // Test flatten
    static_assert(std::is_same<Flatten::Call<void>, void>(),
        "Testing flatten on empty list");
    static_assert(std::is_same<Flatten::Call<make_t<make_t<>>>, void>(),
        "Testing flatten on a list containing an empty list");
    static_assert(std::is_same<Flatten::Call<make_t<make_t<make_t<>>>>, void>(),
        "Testing flatten on a list containing a list containing an empty list");
    static_assert(std::is_same<Flatten::Call<make_t<int, float>>, make_t<int, float>>(),
        "Testing flatten on a flat list");
    static_assert(
        std::is_same<
            Flatten::Call<make_t<make_t<int, float>, make_t<int, float>>>,
            make_t<int, float, int, float>
        >(),
        "Testing flatten on a 2D list");
    static_assert(
        std::is_same<
            Flatten::Call<make_t<make_t<int, float>, char, make_t<int, float>>>,
            make_t<int, float, char, int, float>
        >(),
        "Testing flatten on a mixed list");
    static_assert(
        std::is_same<
            Flatten::Call<make_t<make_t<make_t<int>, make_t<float, int>>, make_t<make_t<float, double>>>>,
            make_t<int, float, int, float, double>
        >(),
        "Testing flatten on a 3D list");

    // Testing ZipApply
    static_assert(std::is_same<
        ZipApply::Call< make_t<IsZero, IsZero, Factorial>, make_t<Int<0>, Int<1>, Int<3>> >,
        make_t<Bool<true>, Bool<false>, Int<6>>
    >(),
    "Testing Zip Apply");
}
