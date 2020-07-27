#define CORETEST_IMPLEMENT_WITHOUT_MAIN

#include "../coretest/coretest.hpp"

// Pass

TEST(require_true) {
    REQUIRE_TRUE(1);
}

TEST(require_false) {
    REQUIRE_FALSE(0);
}

TEST(require_equal) {
    REQUIRE_EQUAL(1, 1);
    REQUIRE_EQUAL("foo", "foo");
    REQUIRE_EQUAL('a', 'a');
}

TEST(require_not_equal) {
    REQUIRE_NOT_EQUAL(1, 2);
}

TEST(require_less) {
    REQUIRE_LESS(1, 2);
}

TEST(require_less_equal) {
    REQUIRE_LESS_EQUAL(1, 1);
    REQUIRE_LESS_EQUAL(1, 2);
}

TEST(require_greater) {
    REQUIRE_GREATER(2, 1);
}

TEST(require_greater_equal) {
    REQUIRE_GREATER_EQUAL(1, 1);
    REQUIRE_GREATER_EQUAL(2, 1);
}

TEST(float_type) {
    float a = 1.;
    float b = 1.;
    REQUIRE_EQUAL(a, b);
}

TEST(double_type) {
    double a = 1.;
    double b = 1.;
    REQUIRE_EQUAL(a, b);
}

TEST(long_double_type) {
    long double a = 1.;
    long double b = 1.;
    REQUIRE_EQUAL(a, b);
}

TEST(signed_char_type) {
    signed char a = 1;
    signed char b = 1;
    REQUIRE_EQUAL(a, b);
}

TEST(unsigned_char_type) {
    unsigned char a = 1;
    unsigned char b = 1;
    REQUIRE_EQUAL(a, b);
}

TEST(short_type) {
    short a = 1;
    short b = 1;
    REQUIRE_EQUAL(a, b);
}

TEST(unsigned_short_type) {
    unsigned short a = 1;
    unsigned short b = 1;
    REQUIRE_EQUAL(a, b);
}

TEST(int_type) {
    int a = 1;
    int b = 1;
    REQUIRE_EQUAL(a, b);
}

TEST(unsigned_int_type) {
    unsigned int a = 1;
    unsigned int b = 1;
    REQUIRE_EQUAL(a, b);
}

TEST(long_type) {
    long a = 1;
    long b = 1;
    REQUIRE_EQUAL(a, b);
}

TEST(unsigned_long_type) {
    unsigned long a = 1;
    unsigned long b = 1;
    REQUIRE_EQUAL(a, b);
}

TEST(double_evaluation) {
    int i = 0;
    REQUIRE_EQUAL(i++, 0);
    REQUIRE_EQUAL(i, 1);
}

int main(int argc, char **argv) {
    coretest::run_main(argc, argv);
}