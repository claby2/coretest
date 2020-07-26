#include "../coretest/coretest.hpp"

int add(int a, int b) {
    return (a + b);
}

TEST(add_test) {
    REQUIRE_EQUAL(add(1, 2), 3);
    REQUIRE_EQUAL(add(3, 4), 7);
    REQUIRE_EQUAL(add(20, 30), 50);
}