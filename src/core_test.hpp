#ifndef CORE_TEST_HPP
#define CORE_TEST_HPP

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

namespace coretest {
enum class AssertionType {
    REQUIRE,
    CHECK
};

enum class ComparisonType {
    EQUAL,
    TRUE,
    FALSE,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL
};

// Error handling
struct CoreTestError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct Test {
    std::function<void()> function;
    std::string test_name;
    std::string file_name;
    int line_number;
};

struct Check {
    std::string first;
    std::string second;
    std::string suffix;
    std::string comparison_operator;
};

struct PushTest {
    PushTest(std::function<void()> &&test, std::string test_name, std::string file_name, int line_number);
};

const int separator_length = 80;
// Holds all tests
std::vector<Test> tests;
// Holds test names
std::map<std::string, int> test_names;
// Holds user specifed tests to run
std::vector<std::string> specified_tests;
// Holds the checks which have failed
std::vector<Check> failed_checks;
// Number of assertions
int assertions = 0;

PushTest::PushTest(std::function<void()> &&test, std::string test_name, std::string file_name, int line_number) {
    tests.push_back({test, test_name, file_name, line_number});
    test_names[test_name] = tests.size();
}

// Return std::string based on type
template <typename T>
std::string get_string(T var) {
    // Argument given is numeric value, return std::to_string
    return std::to_string(var);
}

template <>
std::string get_string(std::string var) {
    // Argument given is already a string, simply return
    return var;
}

template <>
std::string get_string(const char *var) {
    // Argument given is const char*, convert to string
    std::string new_var = var;
    return new_var;
}

template <>
std::string get_string(char var) {
    // Argument given is a char, return with string constructor
    return std::string(1, var);
}

void exception_throw(AssertionType assertion_type, ComparisonType comparison_type, std::string first, std::string second = "") {
    std::string suffix;
    std::string comparison_operator;
    switch (comparison_type) {
        case ComparisonType::EQUAL:
            comparison_operator = " == ";
            suffix = "EQUAL";
            break;
        case ComparisonType::TRUE:
            second = "true";
            comparison_operator = " == ";
            suffix = "TRUE";
            break;
        case ComparisonType::FALSE:
            second = "false";
            comparison_operator = " == ";
            suffix = "FALSE";
            break;
        case ComparisonType::NOT_EQUAL:
            comparison_operator = " != ";
            suffix = "NOT_EQUAL";
            break;
        case ComparisonType::LESS:
            comparison_operator = " < ";
            suffix = "LESS";
            break;
        case ComparisonType::LESS_EQUAL:
            comparison_operator = " <= ";
            suffix = "LESS_EQUAL";
            break;
        case ComparisonType::GREATER:
            comparison_operator = " > ";
            suffix = "GREATER";
            break;
        case ComparisonType::GREATER_EQUAL:
            comparison_operator = " >= ";
            suffix = "GREATER_EQUAL";
            break;
    }
    switch (assertion_type) {
        case AssertionType::REQUIRE:
            throw coretest::CoreTestError("REQUIRE_" + suffix + "( " + first + comparison_operator + second + " )");
            break;
        case AssertionType::CHECK:
            failed_checks.push_back({first, second, suffix, comparison_operator});
            break;
    }
}

// Output functions

void print_test(Test test) {
    std::cout << std::string(separator_length, '-') << '\n';
    std::cout << test.test_name << " ( " << test.file_name << ":" << test.line_number << " )" << '\n';
}

void print_failed_assertion(std::string message) {
    std::cout << "FAILED:" << '\n';
    std::cout << std::string(4, ' ') << message << '\n';
}

void print_failed_checks(int initial_checks_failed, int &assertions_failed) {
    for (int i = initial_checks_failed; i < failed_checks.size(); i++) {
        // Iterate through new failed checks
        Check check = failed_checks[i];
        print_failed_assertion("CHECK_" + check.suffix + "( " + check.first + check.comparison_operator + check.second + " )");
    }
    assertions_failed += (failed_checks.size() - initial_checks_failed);
}

void print_results(int tests_failed, int assertions_failed) {
    std::cout << '\n';
    if (tests_failed > 0) {
        // Get maximum string size for counts for output alignment
        int tests_count_size = std::to_string(tests.size()).size();
        int assertions_count_size = std::to_string(assertions).size();
        int max_size = std::max(tests_count_size, assertions_count_size);
        std::cout << std::string(separator_length, '=') << '\n';
        std::cout << "test cases: " << tests.size() << std::string((max_size - tests_count_size), ' ') << " | " << tests_failed << " failed" << '\n';
        std::cout << "assertions: " << assertions << std::string((max_size - assertions_count_size), ' ') << " | " << assertions_failed << " failed" << '\n';
    } else {
        // All tests have passed
        std::cout << std::string(separator_length, '=') << '\n';
        std::cout << "All tests passed ( " << assertions << " assertions in " << tests.size() << " test cases )" << '\n';
    }
}

void run_tests() {
    std::cout << '\n';
    int tests_failed = 0;
    int assertions_failed = 0;
    int initial_checks_failed = 0;
    if (specified_tests.size() > 0) {
        // User has supplied specific tests to run
        std::vector<Test> new_tests;
        for (std::string test_name : specified_tests) {
            for (auto test : tests) {
                if (test.test_name == test_name) {
                    new_tests.push_back(test);
                }
            }
        }
        tests = new_tests;
    }
    for (auto test : tests) {
        initial_checks_failed = failed_checks.size();
        bool has_catch = false;
        try {
            test.function();
        } catch (CoreTestError &e) {
            has_catch = true;
            // Print test case
            print_test(test);
            // Print failed checks
            print_failed_checks(initial_checks_failed, assertions_failed);
            // Print failed assertion
            print_failed_assertion(e.what());
            assertions_failed++;
            tests_failed++;
        }
        if (!has_catch && (failed_checks.size() - initial_checks_failed) > 0) {
            // Checks have failed in the test case but no error was thrown (no REQUIRE)
            // Print test case
            print_test(test);
            // Print failed checks
            print_failed_checks(initial_checks_failed, assertions_failed);
            tests_failed++;
        }
    }
    print_results(tests_failed, assertions_failed);
}

void list_tests() {
    std::cout << '\n';
    std::cout << "All available test cases:" << '\n';
    for (auto test : tests) {
        std::cout << std::string(4, ' ') << test.test_name << '\n';
    }
}
};  // namespace coretest

// Define test case macro
#define TEST(test_name)                                                                      \
    void test##test_name();                                                                  \
    coretest::PushTest PushTest##test_name{test##test_name, #test_name, __FILE__, __LINE__}; \
    void test##test_name()

// Define section macro
#define SECTION(section_name)

// Define assertion macros

#define REQUIRE_EQUAL(first, second)                                \
    coretest::assertions++;                                         \
    if (first != second) {                                          \
        coretest::exception_throw(coretest::AssertionType::REQUIRE, \
                                  coretest::ComparisonType::EQUAL,  \
                                  coretest::get_string(first),      \
                                  coretest::get_string(second));    \
    }

#define REQUIRE_TRUE(first)                                         \
    coretest::assertions++;                                         \
    if (first == false) {                                           \
        coretest::exception_throw(coretest::AssertionType::REQUIRE, \
                                  coretest::ComparisonType::TRUE,   \
                                  coretest::get_string(first));     \
    }

#define REQUIRE_FALSE(first)                                        \
    coretest::assertions++;                                         \
    if (first == true) {                                            \
        coretest::exception_throw(coretest::AssertionType::REQUIRE, \
                                  coretest::ComparisonType::FALSE,  \
                                  coretest::get_string(first));     \
    }

#define REQUIRE_NOT_EQUAL(first, second)                               \
    coretest::assertions++;                                            \
    if (first == second) {                                             \
        coretest::exception_throw(coretest::AssertionType::REQUIRE,    \
                                  coretest::ComparisonType::NOT_EQUAL, \
                                  coretest::get_string(first),         \
                                  coretest::get_string(second));       \
    }

#define REQUIRE_LESS(first, second)                                 \
    coretest::assertions++;                                         \
    if (first >= second) {                                          \
        coretest::exception_throw(coretest::AssertionType::REQUIRE, \
                                  coretest::ComparisonType::LESS,   \
                                  coretest::get_string(first),      \
                                  coretest::get_string(second));    \
    }

#define REQUIRE_LESS_EQUAL(first, second)                               \
    coretest::assertions++;                                             \
    if (first > second) {                                               \
        coretest::exception_throw(coretest::AssertionType::REQUIRE,     \
                                  coretest::ComparisonType::LESS_EQUAL, \
                                  coretest::get_string(first),          \
                                  coretest::get_string(second));        \
    }

#define REQUIRE_GREATER(first, second)                               \
    coretest::assertions++;                                          \
    if (first <= second) {                                           \
        coretest::exception_throw(coretest::AssertionType::REQUIRE,  \
                                  coretest::ComparisonType::GREATER, \
                                  coretest::get_string(first),       \
                                  coretest::get_string(second));     \
    }

#define REQUIRE_GREATER_EQUAL(first, second)                               \
    coretest::assertions++;                                                \
    if (first < second) {                                                  \
        coretest::exception_throw(coretest::AssertionType::REQUIRE,        \
                                  coretest::ComparisonType::GREATER_EQUAL, \
                                  coretest::get_string(first),             \
                                  coretest::get_string(second));           \
    }

#define CHECK_EQUAL(first, second)                                 \
    coretest::assertions++;                                        \
    if (first != second) {                                         \
        coretest::exception_throw(coretest::AssertionType::CHECK,  \
                                  coretest::ComparisonType::EQUAL, \
                                  coretest::get_string(first),     \
                                  coretest::get_string(second));   \
    }

#define CHECK_TRUE(first)                                         \
    coretest::assertions++;                                       \
    if (first == false) {                                         \
        coretest::exception_throw(coretest::AssertionType::CHECK, \
                                  coretest::ComparisonType::TRUE, \
                                  coretest::get_string(first));   \
    }

#define CHECK_FALSE(first)                                         \
    coretest::assertions++;                                        \
    if (first == true) {                                           \
        coretest::exception_throw(coretest::AssertionType::CHECK,  \
                                  coretest::ComparisonType::FALSE, \
                                  coretest::get_string(first));    \
    }

#define CHECK_NOT_EQUAL(first, second)                                 \
    coretest::assertions++;                                            \
    if (first == second) {                                             \
        coretest::exception_throw(coretest::AssertionType::CHECK,      \
                                  coretest::ComparisonType::NOT_EQUAL, \
                                  coretest::get_string(first),         \
                                  coretest::get_string(second));       \
    }

#define CHECK_LESS(first, second)                                 \
    coretest::assertions++;                                       \
    if (first >= second) {                                        \
        coretest::exception_throw(coretest::AssertionType::CHECK, \
                                  coretest::ComparisonType::LESS, \
                                  coretest::get_string(first),    \
                                  coretest::get_string(second));  \
    }

#define CHECK_LESS_EQUAL(first, second)                                 \
    coretest::assertions++;                                             \
    if (first > second) {                                               \
        coretest::exception_throw(coretest::AssertionType::CHECK,       \
                                  coretest::ComparisonType::LESS_EQUAL, \
                                  coretest::get_string(first),          \
                                  coretest::get_string(second));        \
    }

#define CHECK_GREATER(first, second)                                 \
    coretest::assertions++;                                          \
    if (first <= second) {                                           \
        coretest::exception_throw(coretest::AssertionType::CHECK,    \
                                  coretest::ComparisonType::GREATER, \
                                  coretest::get_string(first),       \
                                  coretest::get_string(second));     \
    }

#define CHECK_GREATER_EQUAL(first, second)                                 \
    coretest::assertions++;                                                \
    if (first < second) {                                                  \
        coretest::exception_throw(coretest::AssertionType::CHECK,          \
                                  coretest::ComparisonType::GREATER_EQUAL, \
                                  coretest::get_string(first),             \
                                  coretest::get_string(second));           \
    }

// Standard main entry point
int main(int argc, char **argv) {
    bool list_tests = false;
    if (argc > 1) {
        // Parse args
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg[0] == '[' && arg[arg.length() - 1] == ']') {
                // Get test name without [ ] chars
                std::string test_name = arg.substr(1, arg.length() - 2);
                // Remove whitespace
                test_name.erase(std::remove(test_name.begin(), test_name.end(), ' '), test_name.end());
                coretest::specified_tests.push_back(test_name);
            } else if (arg == "-l" || arg == "--list-tests") {
                // List tests
                list_tests = true;
            }
        }
    }
    if (list_tests) {
        coretest::list_tests();
    } else {
        coretest::run_tests();
    }
}
#endif