#include <type_traits>

#include "functions.hpp"
#include "functions_test.hpp"

template <typename Type>
using CurriedIsInt = typename Curry<IsIntF>::template Call<Type>;

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
    static_assert(IsInt<int>::Type::Value,
        "Testing that is_int correctly returns true");
    static_assert(!IsInt<bool>::Type::Value,
        "Testing that is_int correctly returns false");

    // Test select_t
    static_assert(std::is_same<select_t<Bool<true>, int, float>, int>(),
        "Testing that select correctly returns first");
    static_assert(std::is_same<select_t<Bool<false>, int, float>, float>(),
        "Testing that select correctly returns second");

    // Test call
    static_assert(IsIntF::Call<int>::Value,
        "Testing that IsIntF(int) correctly returns true");
    static_assert(call<IsIntF, int>::Value,
        "Testing that call<IsIntF, int> correctly returns true");
    static_assert(!IsIntF::Call<float>::Value,
        "Testing that IsIntF(float) correctly returns false");
    static_assert(!call<IsIntF, float>::Value,
        "Testing that call<IsIntF, float> correctly returns false");
    static_assert(std::is_same<call<SelectF, Bool<true>, int, float>, int>(),
        "Testing that call<SelectF, true> correctly selects the first value");
    static_assert(std::is_same<call<SelectF, Bool<false>, int, float>, float>(),
        "Testing that call<SelectF, false> correctly selects the second value");

    // Test Curry
    static_assert(Curry<IsIntF>::Call<int>::Type::Value,
        "Testing that curry with all arguments supplied returns the correct result (true)");
    static_assert(!Curry<IsIntF>::Call<float>::Type::Value,
        "Testing that curry with all arguments supplied returns the correct result (false)");
    static_assert(CurriedIsInt<int>::Type::Value,
        "Testing that curry can be used to create new functions which return the correct result (true) when called");
    static_assert(!CurriedIsInt<float>::Type::Value,
        "Testing that curry can be used to create new functions which return the correct result (false) when called");


}
