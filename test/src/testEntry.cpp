#include <type_traits>

#include "type_list.hpp"

int main()
{
    static_assert(std::is_same<head< List<int, void> >, int>(),
        "Testing head on small list");
    static_assert(std::is_same<head< List<float, List<int, void>> >, float>(),
        "Testing head on larger list");
    static_assert(std::is_same<tail< List<int, void> >, void>(),
        "Testing tail on small list");
    static_assert(std::is_same<tail< List<float, List<int, void>> >, List<int, void>>(),
        "Testing tail on larger list");

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
}
