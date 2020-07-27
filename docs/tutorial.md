# Tutorial

## Contents

- [Installation](#installation)
- [Implementing tests](#implementing-tests)
- [Implementing sections](#implementing-sections)

## Installation

1. Download the header in [`/src`](https://github.com/claby2/coretest/tree/master/coretest)
2. Either put the header file in a central location (with a specified path) or directly in your project tree.

## Implementing tests

Here is an example of a function that we can test.
This function takes two numbers and returns the value of the two numbers added together.

```cpp
int add(int a, int b) {
    return (a + b);
}
```

Here is what the test file may look like.

[`tutorial_tests.cpp`](../examples/tutorial_sections.cpp):

```cpp
#include <coretest/coretest.hpp>

int add(int a, int b) {
    return (a + b);
}

TEST(add_test) {
    REQUIRE_EQUAL(add(1, 2), 3);
    REQUIRE_EQUAL(add(3, 4), 7);
    REQUIRE_EQUAL(add(20, 30), 50);
}
```

After this file is compiled to an executable, it would run the tests and report any failures.
The behavior of the tests can be changed using command line arguments.
Keep in mind that the function that is being tested is included in the file for example purposes.

## Implementing sections

Along with multiple test cases, `coretest` allows multiple sections.

We can extend the previous test file by adding another function that checks if a number is even.

[`tutorial_sections.cpp`](../examples/tutorial_sections.cpp):

```cpp
#include <coretest/coretest.hpp>

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
```

[Home](./readme.md)
