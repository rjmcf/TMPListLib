#include <type_traits>

#include "old_functions.hpp"
#include "old_functions_test.hpp"

void test_old_functions()
{
    // Testing Is0
    static_assert(Is0<Int<0>>::Ret::Value,
        "Testing that Is0 returns true correctly");
    static_assert(!Is0<Int<2>>::Ret::Value,
        "Testing that Is0 returns true correctly");

    // Testing select
    static_assert(std::is_same<select<Bool<true>, int, float>, int>(),
        "Testing that Select correctly returns first");
    static_assert(std::is_same<select<Bool<false>, int, float>, float>(),
        "Testing that Select correctly returns second");

    // Testing Filter
    static_assert(std::is_same<
        filter<Is0, make_t<Int<0>, Int<4>, Int<5>, Int<0>> >,
        make_t<Int<0>, Int<0>>
        >(),
        "Testing filter");

    // normal lists of functions won't work
    //make_t<Is0> badList;

    // Testing function list
    static_assert(std::is_same<
        FList<Is0, FList<Is0, void>>::Tail,
        FList<Is0, void>
        >(),
        "Testing function lists");
    static_assert(FList<Is0, void>::Head<Int<0>>::Ret::Value,
        "Testing calling a function correctly returns true");
    static_assert(!FList<Is0, void>::Head<Int<10>>::Ret::Value,
        "Testing calling a function correctly returns false");

    // Even FLists of functions that take functions won't work
    //FList<Filter, void> badFList;

    // Supposedly template template template arguments are standards compliant
    // but they're also a bit silly. The fact that functions were different from
    // values and functions that take functions were different from normal functions
    // is why I moved away from this implementation
    using TestFFList = FFList<Filter, void>;
    static_assert(std::is_same<TestFFList, TestFFList>(),
        "Testing FFList can store Filter");
}
