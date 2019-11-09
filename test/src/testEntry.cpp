#include <type_traits>

#include "type_list.hpp"

// Test function, returns Bool<true> if passed int type.
struct IsInt
{
    template<typename T>
    using Call = Bool<std::is_same<T, int>::value>;
};

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

    // Test make_t
    static_assert(std::is_same<make_t<>, void>(),
        "Testing making an empty list");
    static_assert(std::is_same<make_t<int>, List<int, void>>(),
        "Testing making a single element list");
    static_assert(
        std::is_same<
            make_t<int, float, char>,
            List<int, List<float, List<char, void>>>
        >(),
        "Testing making a longer list");
    static_assert(std::is_same<head< make_t<int, float, char> >, int>(),
        "Testing head applied to make_t");
    static_assert(std::is_same<tail< make_t<int, float, char> >, make_t<float, char> >(),
        "Testing tail applied to make_t");

    // Test append
    static_assert(std::is_same<append<int, void>, make_t<int>>(),
        "Testing append on empty list");
    static_assert(std::is_same<append<float, make_t<int>>, make_t<int, float>>(),
        "Testing append on small list");
    static_assert(
        std::is_same<
            append<float, make_t<char, int> >,
            make_t<char, int, float>
        >(),
        "Testing append on larger list");
    static_assert(std::is_same<AppendF::Call<int, void>, append<int, void>>(),
        "Testing that AppendF::Call works the same as append on an empty list");
    static_assert(std::is_same<AppendF::Call<int, make_t<float>>, append<int, make_t<float>> >(),
        "Testing that AppendF::Call works the same as append on a small list");

    // Test concat
    static_assert(std::is_same<concat<void, void>, void>(),
        "Testing concat on two empty lists");
    static_assert(std::is_same<concat<void, make_t<int>>, make_t<int> >(),
        "Testing concat when first list empty");
    static_assert(std::is_same<concat<make_t<int>, void>, make_t<int> >(),
        "Testing concat when second list empty");
    static_assert(std::is_same<concat<make_t<int>, make_t<float>>, make_t<int, float> >(),
        "Testing concat on two small lists");
    static_assert(
        std::is_same<
            concat< make_t<int, char>, make_t<float, double> >,
            make_t<int, char, float, double>
        >(),
        "Testing concat on two larger lists");
    static_assert(
        std::is_same<
            ConcatF::Call< make_t<int, char>, make_t<float, double> >,
            concat< make_t<int, char>, make_t<float, double> >
        >(),
        "Testing ConcatF::Call works the same as concat");

    // Test value semantics
    static_assert(Val<int, 4>::Value == 4,
        "Testing that Val can store an int");
    static_assert(Val<char, 'r'>::Value == 'r',
        "Testing that Val can store a char");
    static_assert(head< make_t<Val<int,45>> >::Value == 45,
        "Testing that Lists can store Vals");
    static_assert(head<tail< make_t<Val<int, 17>, Val<int, 42>> >>::Value == 42,
        "Testing that Lists can store Vals in their tails");

    // Test call
    static_assert(IsInt::Call<int>::Value,
        "Testing that IsInt(int) correctly returns true");
    static_assert(call<IsInt, int>::Value,
        "Testing that call<IsInt, int> correctly returns true");
    static_assert(!IsInt::Call<float>::Value,
        "Testing that IsInt(float) correctly returns false");
    static_assert(!call<IsInt, float>::Value,
        "Testing that call<IsInt, float> correctly returns false");

    // Test lists of functions
    static_assert(std::is_same<make_t<IsInt>, List<IsInt, void> >(),
        "Test that make_t applies to functions too");
    static_assert(call<head<make_t<IsInt>>, int>::Value,
        "Test that the head of a list of functions can be called");
}
