#include <type_traits>

#include "type_list.hpp"
#include "type_list_test.hpp"

void test_type_lists()
{
    // Test list structure
    static_assert(std::is_same<head< List<int, void> >, int>(),
        "Testing head on small list");
    static_assert(std::is_same<head< List<float, List<int, void>> >, float>(),
        "Testing head on larger list");
    static_assert(std::is_same<tail< List<int, void> >, void>(),
        "Testing tail on small list");
    static_assert(std::is_same<tail< List<float, List<int, void>> >, List<int, void>>(),
        "Testing tail on larger list");

    // Test make_t
    static_assert(std::is_same<make_t<>, void>(),
        "Testing making an empty list");
    static_assert(std::is_same<make_t<int>, List<int, void>>(),
        "Testing making a single element list");
    static_assert(
        std::is_same<
            make_t<int, float, char>,
            List<int, List<float, List<char, void>>>
        >(),
        "Testing making a longer list");
    static_assert(std::is_same<head< make_t<int, float, char> >, int>(),
        "Testing head applied to make_t");
    static_assert(std::is_same<tail< make_t<int, float, char> >, make_t<float, char> >(),
        "Testing tail applied to make_t");

    // Test multi-dimensional lists
    static_assert(std::is_same<make_t<make_t<>>, List<void,void> >(),
        "Testing the structure of a list containing an empty list");
    static_assert(std::is_same<make_t<make_t<int>>, List< List<int,void>, void> >(),
        "Testing the structure of a list containing one single element list");
    static_assert(std::is_same<make_t<make_t<int, float>>, List< make_t<int,float>, void> >(),
        "Testing the structure of a list containing one multi_element list");
    static_assert(
        std::is_same<
            make_t<make_t<int, float>, make_t<char, double>>,
            List< make_t<int,float>, List<make_t<char, double>,void> >
        >(),
        "Testing the structure of a list containing many multi_element lists");
}
