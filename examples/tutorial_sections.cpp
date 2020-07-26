#include "../coretest/coretest.hpp"

int add(int a, int b) {
    return (a + b);
}

bool is_even(int a) {
    return (a % 2 == 0);
}

TEST(add_test) {
    REQUIRE_EQUAL(add(1, 2), 3);
    REQUIRE_EQUAL(add(3, 4), 7);
    REQUIRE_EQUAL(add(20, 30), 50);
}

TEST(is_even_test) {
    SECTION(test_even) {
        int a = 100;
        REQUIRE_TRUE(is_even(a));
    }
    SECTION(test_odd) {
        int a = 99;
        REQUIRE_FALSE(is_even(a));
    }
}