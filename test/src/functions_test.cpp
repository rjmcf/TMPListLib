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

    // Test IsInt
    static_assert(IsInt::Call<int>::Value,
        "Testing that is_int correctly returns true");
    static_assert(!IsInt::Call<bool>::Value,
        "Testing that is_int correctly returns false");

    // Test select_t
    static_assert(std::is_same<select_t<Bool<true>, int, float>, int>(),
        "Testing that select correctly returns first");
    static_assert(std::is_same<select_t<Bool<false>, int, float>, float>(),
        "Testing that select correctly returns second");

    // Test call
    static_assert(IsInt::Call<int>::Value,
        "Testing that IsInt(int) correctly returns true");
    static_assert(call<IsInt, int>::Value,
        "Testing that call<IsInt, int> correctly returns true");
    static_assert(!IsInt::Call<float>::Value,
        "Testing that IsInt(float) correctly returns false");
    static_assert(!call<IsInt, float>::Value,
        "Testing that call<IsInt, float> correctly returns false");
    static_assert(std::is_same<call<SelectF, Bool<true>, int, float>, int>(),
        "Testing that call<IsInt, float> correctly returns false");
    static_assert(std::is_same<call<SelectF, Bool<false>, int, float>, float>(),
        "Testing that call<IsInt, float> correctly returns false");
}
