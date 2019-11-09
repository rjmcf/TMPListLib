#include <type_traits>

#include "type_list.hpp"

int main()
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

    // Test append
    static_assert(std::is_same<append<int, void>, List<int, void>>(),
        "Testing append on empty list");
    static_assert(std::is_same<append<float, List<int,void>>, List<int, List<float, void>>>(),
        "Testing append on small list");
    static_assert(
        std::is_same<
            append<float, List<char, List<int,void>> >,
            List<char, List<int, List<float, void>>>
        >(),
        "Testing append on larger list");
    static_assert(std::is_same<AppendF::Call<int, void>, append<int, void>>(),
        "Testing that AppendF::Call works the same as append on an empty list");
    static_assert(std::is_same<AppendF::Call<int, List<float,void>>, append<int, List<float, void>> >(),
        "Testing that AppendF::Call works the same as append on a small list");

    // Test concat
    static_assert(std::is_same<concat<void, void>, void>(),
        "Testing concat on two empty lists");
    static_assert(std::is_same<concat<void, List<int, void>>, List<int, void> >(),
        "Testing concat when first list empty");
    static_assert(std::is_same<concat<List<int, void>, void>, List<int, void> >(),
        "Testing concat when second list empty");
    static_assert(
        std::is_same<
            concat< List<int, void>, List<float, void> >,
            List<int, List<float, void>>
        >(),
        "Testing concat on two small lists");
    static_assert(
        std::is_same<
            concat< List<int, List<char, void>>, List<float, List<double, void>> >,
            List<int, List<char, List<float, List<double, void>>>>
        >(),
        "Testing concat on two larger lists");
    static_assert(
        std::is_same<
            ConcatF::Call< List<int, List<char, void>>, List<float, List<double, void>> >,
            concat< List<int, List<char, void>>, List<float, List<double, void>> >
        >(),
        "Testing ConcatF::Call works the same as concat");
}
