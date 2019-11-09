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
}
