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

    static_assert(is_convertible<AnyType, ListType<IntType>>(),
        "Test that AnyType is convertible to IntList");
    static_assert(is_convertible<AnyType, ListType<BoolType>>(),
        "Test that AnyType is convertible to BoolList");
    static_assert(!is_convertible<IntType, ListType<IntType>>(),
        "Test that IntType is not convertible to IntList");
    static_assert(!is_convertible<ListType<BoolType>, ListType<IntType>>(),
        "Test that BoolList is not convertible to IntList");
    static_assert(is_convertible<ListType<IntType>, ListType<IntType>>(),
        "Test that IntList is convertible to IntList");
    static_assert(is_convertible<ListType<AnyType>, ListType<IntType>>(),
        "Test that List of AnyType is convertible to IntList");
    static_assert(!is_convertible<ListType<AnyType>, IntType>(),
        "Test that List of AnyType is not convertible to Int");

    static_assert(std::is_same<convert_to<AnyType, ListType<IntType>>, ListType<IntType>>(),
        "Test that AnyType converts to IntList");
    static_assert(std::is_same<convert_to<AnyType, ListType<BoolType>>, ListType<BoolType>>(),
        "Test that AnyType converts to BoolList");
    static_assert(std::is_same<convert_to<ListType<IntType>, ListType<IntType>>, ListType<IntType>>(),
        "Test that IntList converts to IntList");
    static_assert(std::is_same<convert_to<ListType<AnyType>, ListType<IntType>>, ListType<IntType>>(),
        "Test that List of AnyType converts to IntList");

    static_assert(is_convertible<AnyType, ListType<ListType<IntType>>>(),
        "Test that AnyType is convertible to IntListList");
    static_assert(is_convertible<ListType<AnyType>, ListType<ListType<IntType>>>(),
        "Test that List of AnyType is convertible to IntListList");
    static_assert(is_convertible<ListType<ListType<AnyType>>, ListType<ListType<IntType>>>(),
        "Test that List of Lists of AnyType is convertible to IntListList");
    static_assert(!is_convertible<ListType<IntType>, ListType<ListType<IntType>>>(),
        "Test that IntList is not convertible to IntListList");
    static_assert(!is_convertible<ListType<ListType<IntType>>, ListType<IntType>>(),
        "Test that IntListList is not convertible to IntList");

    static_assert(std::is_same<convert_to<AnyType, ListType<ListType<IntType>>>, ListType<ListType<IntType>>>(),
        "Test that AnyType is convertible to IntListList");
    static_assert(std::is_same<convert_to<ListType<AnyType>, ListType<ListType<IntType>>>, ListType<ListType<IntType>>>(),
        "Test that List of AnyType converts to IntListList");
    static_assert(std::is_same<convert_to<ListType<ListType<AnyType>>, ListType<ListType<IntType>>>, ListType<ListType<IntType>>>(),
        "Test that List of Lists of AnyType converts to IntListList");
}
