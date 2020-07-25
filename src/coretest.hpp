#ifndef CORETEST_HPP
#define CORETEST_HPP

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

struct Assertion {
    bool passed;
    AssertionType type;
    ComparisonType comparison;
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
// Holds all assertions
std::vector<Assertion> assertions;
bool show_successful = false;

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

void add_assertion(Assertion &assertion) {
    switch (assertion.comparison) {
        case ComparisonType::EQUAL:
            assertion.comparison_operator = " == ";
            assertion.suffix = "EQUAL";
            break;
        case ComparisonType::TRUE:
            assertion.second = "true";
            assertion.comparison_operator = " == ";
            assertion.suffix = "TRUE";
            break;
        case ComparisonType::FALSE:
            assertion.second = "false";
            assertion.comparison_operator = " == ";
            assertion.suffix = "FALSE";
            break;
        case ComparisonType::NOT_EQUAL:
            assertion.comparison_operator = " != ";
            assertion.suffix = "NOT_EQUAL";
            break;
        case ComparisonType::LESS:
            assertion.comparison_operator = " < ";
            assertion.suffix = "LESS";
            break;
        case ComparisonType::LESS_EQUAL:
            assertion.comparison_operator = " <= ";
            assertion.suffix = "LESS_EQUAL";
            break;
        case ComparisonType::GREATER:
            assertion.comparison_operator = " > ";
            assertion.suffix = "GREATER";
            break;
        case ComparisonType::GREATER_EQUAL:
            assertion.comparison_operator = " >= ";
            assertion.suffix = "GREATER_EQUAL";
            break;
    }
    assertions.push_back(assertion);
    if (assertion.passed == false) {
        switch (assertion.type) {
            case AssertionType::REQUIRE:
                throw coretest::CoreTestError("REQUIRE_" + assertion.suffix + "( " + assertion.first + assertion.comparison_operator + assertion.second + " )");
                break;
        }
    }
}

// Return if any assertion from start_index to end_index have failed
bool has_failed(int start_index, int end_index) {
    for (auto assertion : assertions) {
        if (assertion.passed = false) {
            return true;
        }
    }
    return false;
}

// Output functions

// Print formatted test name with file name and line number
void print_test(Test test) {
    std::cout << std::string(separator_length, '-') << '\n';
    std::cout << test.test_name << " ( " << test.file_name << ":" << test.line_number << " )" << '\n';
}

// Print assertion
void print_assertion(Assertion assertion) {
    if (assertion.passed) {
        std::cout << "PASSED:" << '\n';
    } else {
        std::cout << "FAILED:" << '\n';
    }
    std::cout << std::string(4, ' ');
    if (assertion.type == AssertionType::REQUIRE) {
        std::cout << "REQUIRE_";
    } else if (assertion.type == AssertionType::CHECK) {
        std::cout << "CHECK_";
    }
    std::cout << assertion.suffix << "( " + assertion.first + assertion.comparison_operator + assertion.second + " )" << '\n';
}

// Print failed checks from specified index and increment counter.
// Can be overriden to print passed assertions if show_successful flag is true
void print_checks(int initial_assertions, int &assertions_failed) {
    for (int i = initial_assertions; i < assertions.size(); i++) {
        Assertion assertion = assertions[i];
        if ((show_successful && assertion.passed == true) || (assertion.passed == false && assertion.type == AssertionType::CHECK)) {
            print_assertion(assertion);
            if (assertion.passed == false) {
                assertions_failed++;
            }
        }
    }
}

// Print end results
void print_results(int tests_failed, int assertions_failed) {
    std::cout << '\n';
    if (tests_failed > 0) {
        // Get maximum string size for counts for output alignment
        int tests_count_size = std::to_string(tests.size()).length();
        int assertions_count_size = std::to_string(assertions.size()).length();
        int max_size = std::max(tests_count_size, assertions_count_size);
        std::cout << std::string(separator_length, '=') << '\n';
        std::cout << "test cases: " << tests.size() << std::string((max_size - tests_count_size), ' ') << " | " << tests_failed << " failed" << '\n';
        std::cout << "assertions: " << assertions.size() << std::string((max_size - assertions_count_size), ' ') << " | " << assertions_failed << " failed" << '\n';
    } else {
        // All tests have passed
        std::cout << std::string(separator_length, '=') << '\n';
        std::cout << "All tests passed ( " << assertions.size() << " assertions in " << tests.size() << " test cases )" << '\n';
    }
}

// Run through tests
void run_tests() {
    std::cout << '\n';
    int tests_failed = 0;
    int assertions_failed = 0;
    int initial_assertions = 0;
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
        initial_assertions = assertions.size();
        bool has_catch = false;
        try {
            test.function();
        } catch (CoreTestError &e) {
            has_catch = true;
            // Print test case
            print_test(test);
            // Print failed checks
            print_checks(initial_assertions, assertions_failed);
            // Print failed assertion
            std::cout << "FAILED:" << '\n';
            std::cout << std::string(4, ' ') << e.what() << '\n';
            // Assertions failed increase by 1 to account for thrown error
            assertions_failed++;
            tests_failed++;
        }
        if (!has_catch && has_failed(initial_assertions, assertions.size())) {
            // Checks have failed in the test case but no error was thrown (no failed REQUIRE)
            // Print test case
            print_test(test);
            // Print failed checks
            print_checks(initial_assertions, assertions_failed);
            tests_failed++;
        } else if (!has_catch && show_successful == true) {
            print_test(test);
            print_checks(initial_assertions, assertions_failed);
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

#define REQUIRE_EQUAL(first, second)            \
    {                                           \
        coretest::Assertion assertion = {       \
            ((first == second) ? true : false), \
            coretest::AssertionType::REQUIRE,   \
            coretest::ComparisonType::EQUAL,    \
            coretest::get_string(first),        \
            coretest::get_string(second),       \
        };                                      \
        coretest::add_assertion(assertion);     \
    }

#define REQUIRE_TRUE(first)                   \
    {                                         \
        coretest::Assertion assertion = {     \
            ((first == true) ? true : false), \
            coretest::AssertionType::REQUIRE, \
            coretest::ComparisonType::TRUE,   \
            coretest::get_string(first),      \
        };                                    \
        coretest::add_assertion(assertion);   \
    }

#define REQUIRE_FALSE(first)                   \
    {                                          \
        coretest::Assertion assertion = {      \
            ((first == false) ? true : false), \
            coretest::AssertionType::REQUIRE,  \
            coretest::ComparisonType::FALSE,   \
            coretest::get_string(first),       \
        };                                     \
        coretest::add_assertion(assertion);    \
    }

#define REQUIRE_NOT_EQUAL(first, second)         \
    {                                            \
        coretest::Assertion assertion = {        \
            ((first != second) ? true : false),  \
            coretest::AssertionType::REQUIRE,    \
            coretest::ComparisonType::NOT_EQUAL, \
            coretest::get_string(first),         \
            coretest::get_string(second),        \
        };                                       \
        coretest::add_assertion(assertion);      \
    }

#define REQUIRE_LESS(first, second)            \
    {                                          \
        coretest::Assertion assertion = {      \
            ((first < second) ? true : false), \
            coretest::AssertionType::REQUIRE,  \
            coretest::ComparisonType::LESS,    \
            coretest::get_string(first),       \
            coretest::get_string(second),      \
        };                                     \
        coretest::add_assertion(assertion);    \
    }

#define REQUIRE_LESS_EQUAL(first, second)         \
    {                                             \
        coretest::Assertion assertion = {         \
            ((first <= second) ? true : false),   \
            coretest::AssertionType::REQUIRE,     \
            coretest::ComparisonType::LESS_EQUAL, \
            coretest::get_string(first),          \
            coretest::get_string(second),         \
        };                                        \
        coretest::add_assertion(assertion);       \
    }

#define REQUIRE_GREATER(first, second)         \
    {                                          \
        coretest::Assertion assertion = {      \
            ((first > second) ? true : false), \
            coretest::AssertionType::REQUIRE,  \
            coretest::ComparisonType::GREATER, \
            coretest::get_string(first),       \
            coretest::get_string(second),      \
        };                                     \
        coretest::add_assertion(assertion);    \
    }

#define REQUIRE_GREATER_EQUAL(first, second)         \
    {                                                \
        coretest::Assertion assertion = {            \
            ((first >= second) ? true : false),      \
            coretest::AssertionType::REQUIRE,        \
            coretest::ComparisonType::GREATER_EQUAL, \
            coretest::get_string(first),             \
            coretest::get_string(second),            \
        };                                           \
        coretest::add_assertion(assertion);          \
    }

#define CHECK_EQUAL(first, second)              \
    {                                           \
        coretest::Assertion assertion = {       \
            ((first == second) ? true : false), \
            coretest::AssertionType::CHECK,     \
            coretest::ComparisonType::EQUAL,    \
            coretest::get_string(first),        \
            coretest::get_string(second),       \
        };                                      \
        coretest::add_assertion(assertion);     \
    }

#define CHECK_TRUE(first)                     \
    {                                         \
        coretest::Assertion assertion = {     \
            ((first == true) ? true : false), \
            coretest::AssertionType::CHECK,   \
            coretest::ComparisonType::TRUE,   \
            coretest::get_string(first),      \
        };                                    \
        coretest::add_assertion(assertion);   \
    }

#define CHECK_FALSE(first)                     \
    {                                          \
        coretest::Assertion assertion = {      \
            ((first == false) ? true : false), \
            coretest::AssertionType::CHECK,    \
            coretest::ComparisonType::FALSE,   \
            coretest::get_string(first),       \
        };                                     \
        coretest::add_assertion(assertion);    \
    }

#define CHECK_NOT_EQUAL(first, second)           \
    {                                            \
        coretest::Assertion assertion = {        \
            ((first != second) ? true : false),  \
            coretest::AssertionType::CHECK,      \
            coretest::ComparisonType::NOT_EQUAL, \
            coretest::get_string(first),         \
            coretest::get_string(second),        \
        };                                       \
        coretest::add_assertion(assertion);      \
    }

#define CHECK_LESS(first, second)              \
    {                                          \
        coretest::Assertion assertion = {      \
            ((first < second) ? true : false), \
            coretest::AssertionType::CHECK,    \
            coretest::ComparisonType::LESS,    \
            coretest::get_string(first),       \
            coretest::get_string(second),      \
        };                                     \
        coretest::add_assertion(assertion);    \
    }

#define CHECK_LESS_EQUAL(first, second)           \
    {                                             \
        coretest::Assertion assertion = {         \
            ((first <= second) ? true : false),   \
            coretest::AssertionType::CHECK,       \
            coretest::ComparisonType::LESS_EQUAL, \
            coretest::get_string(first),          \
            coretest::get_string(second),         \
        };                                        \
        coretest::add_assertion(assertion);       \
    }

#define CHECK_GREATER(first, second)           \
    {                                          \
        coretest::Assertion assertion = {      \
            ((first > second) ? true : false), \
            coretest::AssertionType::CHECK,    \
            coretest::ComparisonType::GREATER, \
            coretest::get_string(first),       \
            coretest::get_string(second),      \
        };                                     \
        coretest::add_assertion(assertion);    \
    }

#define CHECK_GREATER_EQUAL(first, second)           \
    {                                                \
        coretest::Assertion assertion = {            \
            ((first >= second) ? true : false),      \
            coretest::AssertionType::CHECK,          \
            coretest::ComparisonType::GREATER_EQUAL, \
            coretest::get_string(first),             \
            coretest::get_string(second),            \
        };                                           \
        coretest::add_assertion(assertion);          \
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
            } else if (arg == "-s" || arg == "--success") {
                // Show successful tests
                coretest::show_successful = true;
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