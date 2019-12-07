#include <type_traits>

#include "functions.hpp"
#include "functions_test.hpp"

void test_functions()
{
    // Test value semantics
    static_assert(Val<int, 4>::Value == 4,
        "Testing that Val can store an int");
    static_assert(Val<char, 'r'>::Value == 'r',
        "Testing that Val can store a char");
    static_assert(Bool<true>::Value,
        "Testing that Bool can store true");
    static_assert(!Bool<false>::Value,
        "Testing that Bool can store false");

    // Test equals
    static_assert(Equals::Call<int, int>::Value,
        "Testing that equals correctly returns true");
    static_assert(!Equals::Call<int, float>::Value,
        "Testing that equals correctly returns false");

    // Test select_t
    static_assert(std::is_same<Select::Call<Bool<true>, int, float>, int>(),
        "Testing that select correctly returns first");
    static_assert(std::is_same<Select::Call<Bool<false>, int, float>, float>(),
        "Testing that select correctly returns second");

    // Test call
    static_assert(Equals::Call<int, int>::Value,
        "Testing that Equals(int, int) correctly returns true");
    static_assert(call<Equals, int, int>::Value,
        "Testing that call<Equals, int, int> correctly returns true");
    static_assert(!Equals::Call<int, float>::Value,
        "Testing that Equals(int, float) correctly returns false");
    static_assert(!call<Equals, int, float>::Value,
        "Testing that call<Equals, int, float> correctly returns false");
    static_assert(std::is_same<call<Select, Bool<true>, int, float>, int>(),
        "Testing that call<Select, true> correctly selects the first value");
    static_assert(std::is_same<call<Select, Bool<false>, int, float>, float>(),
        "Testing that call<Select, false> correctly selects the second value");

    // Test Curry
    static_assert(Curry::Call<Equals>::Call<int>::Call<int>::Call::Value,
        "Testing that curry with all arguments supplied returns the correct result (true)");
    static_assert(!Curry::Call<Equals>::Call<int>::Call<float>::Call::Value,
        "Testing that curry with all arguments supplied returns the correct result (false)");

    // Test IsZero (a curried Equals<Int<0>> function)
    static_assert(IsZero::Call<Int<0>>::Value,
        "Testing that IsZero correctly returns true");
    static_assert(!IsZero::Call<Int<1>>::Value,
        "Testing that IsZero correctly returns false");

    // Test Factorial
    static_assert(Factorial::Call<Int<4>>::Value == 24,
        "Testing factorial");
}
