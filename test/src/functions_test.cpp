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
    static_assert(equals<int, int>::Value,
        "Testing that equals correctly returns true");
    static_assert(!equals<int, float>::Value,
        "Testing that equals correctly returns false");

    // Test select_t
    static_assert(std::is_same<select_t<Bool<true>, int, float>, int>(),
        "Testing that select correctly returns first");
    static_assert(std::is_same<select_t<Bool<false>, int, float>, float>(),
        "Testing that select correctly returns second");

    // Test call
    static_assert(EqualsF::Call<int, int>::Value,
        "Testing that EqualsF(int, int) correctly returns true");
    static_assert(call<EqualsF, int, int>::Value,
        "Testing that call<EqualsF, int, int> correctly returns true");
    static_assert(!EqualsF::Call<int, float>::Value,
        "Testing that EqualsF(int, float) correctly returns false");
    static_assert(!call<EqualsF, int, float>::Value,
        "Testing that call<EqualsF, int, float> correctly returns false");
    static_assert(std::is_same<call<SelectF, Bool<true>, int, float>, int>(),
        "Testing that call<SelectF, true> correctly selects the first value");
    static_assert(std::is_same<call<SelectF, Bool<false>, int, float>, float>(),
        "Testing that call<SelectF, false> correctly selects the second value");

    // Test Curry
    static_assert(Curry<EqualsF>::Call<int>::Call<int>::Type::Value,
        "Testing that curry with all arguments supplied returns the correct result (true)");
    static_assert(!Curry<EqualsF>::Call<int>::Call<float>::Type::Value,
        "Testing that curry with all arguments supplied returns the correct result (false)");

    // Test IsInt (a curried Equals<int> function)
    static_assert(IsInt<int>::Type::Value,
        "Testing that is_int correctly returns true");
    static_assert(!IsInt<bool>::Type::Value,
        "Testing that is_int correctly returns false");
}
