# Command line

## Contents

- [Specifying a test case to run](#specifying-a-test-case-to-run)
- [Other arguments](#other-arguments)

Testing works without any command arguments; however, additional arguments may be given for more control.

Command line arguments are used in the form `<executable> [<test name> ... ] options`.

## Specifying a test case to run

To run a specific test case, the test case name must be wrapped with `[ ]`.

[`tutorial_sections.cpp`](../examples/tutorial_sections.cpp):

```cpp
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
```

For example, the `is_even_test` test case can be run with:

```console
<executable> [is_even_test]
```

## Other arguments

| Query flags                | Description                         |
|----------------------------|-------------------------------------|
| `-?`, `-h`, `--help`       | display usage information           |
| `-l`, `--list-tests`       | list all test cases                 |
| `-o`, `--out` `<filename>` | write output to filename            |
| `-s`, `--success`          | include successful tests in output  |

[Home](./readme.md)
