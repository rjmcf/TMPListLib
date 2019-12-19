#include <type_traits>

#include "type_system.hpp"
#include "type_system_test.hpp"

void test_type_system()
{
    static_assert(is_convertible<AnyType, AnyType>(),
        "Test that AnyType is convertible to itself");
    static_assert(is_convertible<IntType, IntType>(),
        "Test that another type (Int) is convertible to itself");
    static_assert(is_convertible<AnyType, IntType>(),
        "Test that AnyType is convertible to Int");
    static_assert(is_convertible<AnyType, BoolType>(),
        "Test that AnyType is convertible to Bool");
    static_assert(!is_convertible<BoolType, IntType>(),
        "Test that Bool is not convertible to Int");
    static_assert(!is_convertible<IntType, BoolType>(),
        "Test that Int is not convertible to Bool");

    static_assert(std::is_same<convert_to<AnyType, AnyType>, AnyType>(),
        "Test that AnyType converts to itself");
    static_assert(std::is_same<convert_to<IntType, IntType>, IntType>(),
        "Test that another type (Int) converts to itself");
    static_assert(std::is_same<convert_to<AnyType, IntType>, IntType>(),
        "Test that AnyType converts to Int");
    static_assert(std::is_same<convert_to<AnyType, BoolType>, BoolType>(),
        "Test that AnyType converts to Bool");
}
