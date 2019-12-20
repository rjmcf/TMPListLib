#include "functions.hpp"
#include "functions_test.hpp"

#include "testing_utils.hpp"

void test_functions()
{
    // Test Bool types
    static_assert(Bool<true>::Value,
        "Test that Bools can store true");
    static_assert(!Bool<false>::Value,
        "Test that Bools can store false");
    static_assert(is_convertible<Bool<true>::Type, BoolType>(),
        "Test that Bools have type BoolType");

    // Test Int types
    static_assert(Int<4>::Value == 4,
        "Test that Ints can store an int value");
    static_assert(Int<17>::Value == 17,
        "Test that Ints can store a different int value");
    static_assert(is_convertible<Int<4>::Type, IntType>(),
        "Test that Ints have type IntType");

    // Test Not function
    static_assert(!Not::Call<Bool<true>>::Value,
        "Test that Not returns false");
    static_assert(Not::Call<Bool<false>>::Value,
        "Test that Not returns true");
    static_assert(is_convertible<Not::Call<Bool<false>>::Type, BoolType>(),
        "Test that Not returns a BoolType");
#if !AVOID_FAILURE_TESTS
    static_assert(Not::Call<Int<0>>::Value, "FAIL <functions01>:\
        Can't call Not with Int value");
    static_assert(false, "checking <functions01>");
#endif
}
