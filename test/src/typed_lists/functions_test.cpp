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

    // Test And function
    static_assert(And::Call<Bool<true>, Bool<true>>::Value,
        "Test that And returns true");
    static_assert(!And::Call<Bool<false>, Bool<true>>::Value,
        "Test that And returns false when first is false");
    static_assert(!And::Call<Bool<true>, Bool<false>>::Value,
        "Test that And returns false when second is false");
    static_assert(!And::Call<Bool<false>, Bool<false>>::Value,
        "Test that And returns false when both are false");
    static_assert(is_convertible<And::Call<Bool<true>, Bool<true>>::Type, BoolType>(),
        "Test that And returns a BoolType");
#if !AVOID_FAILURE_TESTS
    static_assert(And::Call<Int<1>, Bool<true>>::Value, "FAIL <functions02>:\
        Can't call And with Int value");
    static_assert(false, "checking <functions02>");
    static_assert(And::Call<Bool<true>, Int<1>>::Value, "FAIL <functions03>:\
        Can't call And with Int value");
    static_assert(false, "checking <functions03>");
    static_assert(And::Call<Int<1>, Int<1>>::Value, "FAIL <functions04>:\
        Can't call And with Int values");
    static_assert(false, "checking <functions04>");
#endif
}
