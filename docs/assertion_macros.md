# Assertion macros

There are two levels of assertion:

- `REQUIRE` - the test case would quit if the assertion were to fail.
- `CHECK` - the test case would continue if the assertion were to fail.

Both of the assertions would mark the test case as failed.

## Macros

### REQUIRE_EQUAL(x, y)

Requires that `x` is equal to `y`.

```cpp
TEST(test) {
    int a = 1;
    int b = 1;
    REQUIRE_EQUAL(a, b);
    REQUIRE_EQUAL(a, 1);
    REQUIRE_EQUAL(1, b);
    REQUIRE_EQUAL(1, 1);
    REQUIRE_EQUAL("foo", "foo");
}
```

### REQUIRE_TRUE(x)

Requires that `x` evaluates to `true`.

```cpp
TEST(test) {
    bool a = true;
    REQUIRE_TRUE(a);
    REQUIRE_TRUE(1);
}
```

### REQUIRE_FALSE(x)

Requires that `x` evaluates to `false`.

```cpp
TEST(test) {
    bool a = false;
    REQUIRE_FALSE(a);
    REQUIRE_FALSE(0);
}
```

### REQUIRE_NOT_EQUAL(x, y)

Requires that `x` is not equal to `y`.

```cpp
TEST(test) {
    int a = 1;
    int b = 2;
    REQUIRE_NOT_EQUAL(a, b);
    REQUIRE_NOT_EQUAL(a, 2);
    REQUIRE_NOT_EQUAL(1, b);
    REQUIRE_NOT_EQUAL(1, 2);
    REQUIRE_NOT_EQUAL("foo", "bar");
}
```

### REQUIRE_LESS(x, y)

Requires that `x` is less than `y`.

```cpp
TEST(test) {
    int a = 1;
    int b = 2;
    REQUIRE_LESS(a, b);
    REQUIRE_LESS(a, 2);
    REQUIRE_LESS(1, b);
    REQUIRE_LESS(1, 2);
}
```

### REQUIRE_LESS_EQUAL(x, y)

Requires that `x` is less than or equal to `y`.

```cpp
TEST(test) {
    int a = 1;
    int b = 2;
    REQUIRE_LESS_EQUAL(a, b);
    REQUIRE_LESS_EQUAL(a, 2);
    REQUIRE_LESS_EQUAL(1, b);
    REQUIRE_LESS_EQUAL(1, 2);
    REQUIRE_LESS_EQUAL(a, 1);
    REQUIRE_LESS_EQUAL(1, a);
    REQUIRE_LESS_EQUAL(a, a);
}
```

### REQUIRE_GREATER(x, y)

Requires that `x` is greater than `y`.

```cpp
TEST(test) {
    int a = 2;
    int b = 1;
    REQUIRE_GREATER(a, b);
    REQUIRE_GREATER(a, 1);
    REQUIRE_GREATER(2, b);
    REQUIRE_GREATER(2, 1);
}
```

### REQUIRE_GREATER_EQUAL(x, y)

Requires that `x` is greater than or equal to `y`.

```cpp
TEST(test) {
    int a = 2;
    int b = 1;
    REQUIRE_GREATER_EQUAL(a, b);
    REQUIRE_GREATER_EQUAL(a, 1);
    REQUIRE_GREATER_EQUAL(2, b);
    REQUIRE_GREATER_EQUAL(2, 1);
    REQUIRE_GREATER_EQUAL(a, 1);
    REQUIRE_GREATER_EQUAL(2, a);
    REQUIRE_GREATER_EQUAL(a, a);
}
```

### CHECK_EQUAL(x, y)

Checks that `x` is equal to `y`.

```cpp
TEST(test) {
    int a = 1;
    int b = 1;
    CHECK_EQUAL(a, b);
    CHECK_EQUAL(a, 1);
    CHECK_EQUAL(1, b);
    CHECK_EQUAL(1, 1);
    CHECK_EQUAL("foo", "foo");
}
```

### CHECK_TRUE(x)

Checks that `x` evaluates to `true`.

```cpp
TEST(test) {
    bool a = true;
    CHECK_TRUE(a);
    CHECK_TRUE(1);
}
```

### CHECK_FALSE(x)

Checks that `x` evaluates to `false`.

```cpp
TEST(test) {
    bool a = false;
    CHECK_FALSE(a);
    CHECK_FALSE(0);
}
```

### CHECK_NOT_EQUAL(x, y)

Checks that `x` is not equal to `y`.

```cpp
TEST(test) {
    int a = 1;
    int b = 2;
    CHECK_NOT_EQUAL(a, b);
    CHECK_NOT_EQUAL(a, 2);
    CHECK_NOT_EQUAL(1, b);
    CHECK_NOT_EQUAL(1, 2);
    CHECK_NOT_EQUAL("foo", "bar");
}
```

### CHECK_LESS(x, y)

Checks that `x` is less than `y`.

```cpp
TEST(test) {
    int a = 1;
    int b = 2;
    CHECK_LESS(a, b);
    CHECK_LESS(a, 2);
    CHECK_LESS(1, b);
    CHECK_LESS(1, 2);
}
```

### CHECK_LESS_EQUAL(x, y)

Checks that `x` is less than or equal to `y`.

```cpp
TEST(test) {
    int a = 1;
    int b = 2;
    CHECK_LESS_EQUAL(a, b);
    CHECK_LESS_EQUAL(a, 2);
    CHECK_LESS_EQUAL(1, b);
    CHECK_LESS_EQUAL(1, 2);
    CHECK_LESS_EQUAL(a, 1);
    CHECK_LESS_EQUAL(1, a);
    CHECK_LESS_EQUAL(a, a);
}
```

### CHECK_GREATER(x, y)

Checks that `x` is greater than `y`.

```cpp
TEST(test) {
    int a = 2;
    int b = 1;
    CHECK_GREATER(a, b);
    CHECK_GREATER(a, 1);
    CHECK_GREATER(2, b);
    CHECK_GREATER(2, 1);
}
```

### CHECK_GREATER_EQUAL(x, y)

Checks that `x` is greater than or equal to `y`.

```cpp
TEST(test) {
    int a = 2;
    int b = 1;
    CHECK_GREATER_EQUAL(a, b);
    CHECK_GREATER_EQUAL(a, 1);
    CHECK_GREATER_EQUAL(2, b);
    CHECK_GREATER_EQUAL(2, 1);
    CHECK_GREATER_EQUAL(a, 1);
    CHECK_GREATER_EQUAL(2, a);
    CHECK_GREATER_EQUAL(a, a);
}
```
