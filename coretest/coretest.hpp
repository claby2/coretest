#ifndef CORETEST_HPP
#define CORETEST_HPP

#include <algorithm>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
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

    Assertion(bool new_passed, AssertionType new_type, ComparisonType new_comparison, std::string new_first, std::string new_second = "", std::string new_suffix = "", std::string new_comparison_operator = "") {
        passed = new_passed;
        type = new_type;
        comparison = new_comparison;
        first = new_first;
        second = new_second;
        suffix = new_suffix;
        comparison_operator = new_comparison_operator;
    }
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
std::ofstream file_out;
std::streambuf *coutbuf;

PushTest::PushTest(std::function<void()> &&test, std::string test_name, std::string file_name, int line_number) {
    tests.push_back({test, test_name, file_name, line_number});
    test_names[test_name] = tests.size();
}

// Command line option
class Option {
   public:
    Option(bool &ref) : option_reference(ref) {}

    inline auto &operator[](const std::string name) {
        args.push_back(name);
        return *this;
    }

    inline auto &operator()(std::string new_description, std::string new_name) {
        description = new_description;
        name = new_name;
        return *this;
    }

    bool is_match(std::string argument) {
        for (auto arg : args) {
            if (argument == arg) {
                option_reference = true;
                return true;
            }
        }
        return false;
    }

    inline std::vector<std::string> get_args() {
        return args;
    }

    inline std::string get_description() {
        return description;
    }

    inline void set_require_argument(bool require) {
        require_argument = require;
    }

    inline bool get_require_argument() {
        return require_argument;
    }

    inline void set_argument(std::string new_argument) {
        argument = new_argument;
    }

    inline std::string get_argument() {
        return argument;
    }

    inline std::string get_name() {
        return name;
    }

   private:
    bool &option_reference;
    bool require_argument = false;
    std::vector<std::string> args;
    std::string description;
    std::string argument;
    std::string name;
};

class TestCaseOption {
   public:
    TestCaseOption() {}
    bool is_match(std::string argument) {
        if (argument[0] == '[' && argument[argument.length() - 1] == ']') {
            // Get test name without [ ] chars
            std::string test_name = argument.substr(1, argument.length() - 2);
            // Remove whitespace
            test_name.erase(std::remove(test_name.begin(), test_name.end(), ' '), test_name.end());
            specified_tests.push_back(test_name);
            return true;
        }
        return false;
    }
};

// Declare test case command line argument
TestCaseOption test_case_option;

// Holds command line argument options
std::unordered_map<std::string, Option> options_unordered_map;
bool show_successful = false;
bool show_list = false;
bool show_help = false;
bool send_to_file = false;

template <typename First, typename... Rest>
void add_options(First first, Rest... rest) {
    add_options(first);
    add_options(rest...);
}

template <>
void add_options(Option option) {
    options_unordered_map.insert(std::make_pair(option.get_name(), option));
}

// Initialize options
void initialize_options() {
    Option list(show_list);
    list["-l"]["--list-tests"]("list all test cases", "list");
    Option successful_tests(show_successful);
    successful_tests["-s"]["--success"]("include successful tests in output", "successful_tests");
    Option help(show_help);
    help["-?"]["-h"]["--help"]("display usage information", "help");
    Option out(send_to_file);
    out["-o"]["--out"]("send output to a given file", "out");
    out.set_require_argument(true);
    add_options(list,
                successful_tests,
                help,
                out);
}

// Redirects cout to file
void redirect_cout_to_file() {
    // Get file argument from option
    Option option = options_unordered_map.at("out");
    std::string file_name = option.get_argument();
    // Redirect cout output to file
    file_out.open(file_name);
    coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(file_out.rdbuf());
}

// Return std::string based on type
template <typename T>
inline std::string get_string(T var) {
    // Argument given is numeric value, return std::to_string
    return std::to_string(var);
}

template <>
inline std::string get_string(std::string var) {
    // Argument given is already a string, simply return
    return var;
}

template <>
inline std::string get_string(const char *var) {
    // Argument given is const char*, convert to string
    std::string new_var = var;
    return new_var;
}

template <>
inline std::string get_string(char var) {
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
                throw CoreTestError("Failed require encountered");
                break;
            default:
                break;
        }
    }
}

// Return if any assertion from start_index to end_index have failed
bool has_failed(size_t start_index, size_t end_index) {
    for (size_t i = start_index; i < end_index; i++) {
        if (assertions[i].passed == false) {
            return true;
        }
    }
    return false;
}

// Output functions

void print_option(std::string args, std::string description, int max_argument_length) {
    std::cout << std::string(4, ' ') << args << std::string((max_argument_length - args.length()) + 5, ' ') << description << '\n';
}

void print_help() {
    std::cout << '\n';
    std::cout << "usage:" << '\n';
    std::cout << std::string(4, ' ') << "<executable> [<test name> ... ] options"
              << "\n\n";
    std::cout << "options:" << '\n';
    std::vector<std::string> args;
    std::vector<std::string> descriptions;
    int max_argument_length = -1;
    for (std::pair<std::string, Option> element : options_unordered_map) {
        Option option = element.second;
        // Compose args and description string
        std::string option_args;
        std::string option_description;
        std::vector<std::string> option_args_vector = option.get_args();
        for (size_t i = 0; i < option_args_vector.size(); i++) {
            option_args += option_args_vector[i];
            if (i != option_args_vector.size() - 1) {
                // Not last arg iteration
                option_args += ", ";
            }
        }
        option_description = option.get_description();
        max_argument_length = std::max((int)(option_args.size()), max_argument_length);
        // Push into vectors
        args.push_back(option_args);
        descriptions.push_back(option_description);
    }
    // Print options
    for (size_t i = 0; i < args.size(); i++) {
        print_option(args[i], descriptions[i], max_argument_length);
    }
    std::cout << "\nSee documentation for more information" << '\n';
}

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

// Print failed assertions from specified index and increment fail counter
// Can be overriden to print passed assertions if show_successful flag is true
void print_assertions(int initial_assertions, int &assertions_failed) {
    for (size_t i = initial_assertions; i < assertions.size(); i++) {
        Assertion assertion = assertions[i];
        if (show_successful == true || assertion.passed == false) {
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
            print_assertions(initial_assertions, assertions_failed);
            // Assertions failed increase by 1 to account for thrown error
            tests_failed++;
        }
        if (!has_catch && has_failed(initial_assertions, assertions.size())) {
            // Checks have failed in the test case but no error was thrown (no failed REQUIRE)
            // Print test case
            print_test(test);
            // Print failed checks
            print_assertions(initial_assertions, assertions_failed);
            tests_failed++;
        } else if (!has_catch && show_successful == true) {
            print_test(test);
            print_assertions(initial_assertions, assertions_failed);
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

// Provided main
void run_main(int argc, char **argv) {
    initialize_options();
    if (argc > 1) {
        // Parse args
        for (int i = 0; i < argc; i++) {
            std::string arg = argv[i];
            if (!test_case_option.is_match(arg)) {
                // Option is not in the form of a specified test case
                for (std::pair<std::string, Option> element : options_unordered_map) {
                    Option option = element.second;
                    if (option.is_match(arg)) {
                        if (option.get_require_argument() == true) {
                            // Option requires argument
                            if (i == argc - 1) {
                                // Current argument is last argument, no argument follows
                                throw CoreTestError("Expected argument following " + arg);
                            }
                            std::string option_argument = argv[i + 1];
                            options_unordered_map.at(element.first).set_argument(option_argument);
                            // Increment i to skip next argument
                            i++;
                        }
                        break;
                    }
                }
            }
        }
    }
    if (send_to_file) {
        redirect_cout_to_file();
    }
    if (show_list) {
        list_tests();
    } else if (show_help) {
        print_help();
    } else {
        run_tests();
    }
    if (send_to_file) {
        std::cout.rdbuf(coutbuf);
        file_out.close();
    }
}

}  // namespace coretest

// Define test case macro
#define TEST(test_name)                                                                      \
    void test##test_name();                                                                  \
    coretest::PushTest PushTest##test_name{test##test_name, #test_name, __FILE__, __LINE__}; \
    void test##test_name()

// Define section macro
#define SECTION(section_name)

// Define assertion macros

#define REQUIRE_EQUAL(x, y)                     \
    {                                           \
        auto first = x;                         \
        auto second = y;                        \
        coretest::Assertion assertion = {       \
            ((first == second) ? true : false), \
            coretest::AssertionType::REQUIRE,   \
            coretest::ComparisonType::EQUAL,    \
            coretest::get_string(first),        \
            coretest::get_string(second)};      \
        coretest::add_assertion(assertion);     \
    }

#define REQUIRE_TRUE(x)                       \
    {                                         \
        auto first = x;                       \
        coretest::Assertion assertion = {     \
            ((first == true) ? true : false), \
            coretest::AssertionType::REQUIRE, \
            coretest::ComparisonType::TRUE,   \
            coretest::get_string(first)};     \
        coretest::add_assertion(assertion);   \
    }

#define REQUIRE_FALSE(x)                       \
    {                                          \
        auto first = x;                        \
        coretest::Assertion assertion = {      \
            ((first == false) ? true : false), \
            coretest::AssertionType::REQUIRE,  \
            coretest::ComparisonType::FALSE,   \
            coretest::get_string(first)};      \
        coretest::add_assertion(assertion);    \
    }

#define REQUIRE_NOT_EQUAL(x, y)                  \
    {                                            \
        auto first = x;                          \
        auto second = y;                         \
        coretest::Assertion assertion = {        \
            ((first != second) ? true : false),  \
            coretest::AssertionType::REQUIRE,    \
            coretest::ComparisonType::NOT_EQUAL, \
            coretest::get_string(first),         \
            coretest::get_string(second)};       \
        coretest::add_assertion(assertion);      \
    }

#define REQUIRE_LESS(x, y)                     \
    {                                          \
        auto first = x;                        \
        auto second = y;                       \
        coretest::Assertion assertion = {      \
            ((first < second) ? true : false), \
            coretest::AssertionType::REQUIRE,  \
            coretest::ComparisonType::LESS,    \
            coretest::get_string(first),       \
            coretest::get_string(second)};     \
        coretest::add_assertion(assertion);    \
    }

#define REQUIRE_LESS_EQUAL(x, y)                  \
    {                                             \
        auto first = x;                           \
        auto second = y;                          \
        coretest::Assertion assertion = {         \
            ((first <= second) ? true : false),   \
            coretest::AssertionType::REQUIRE,     \
            coretest::ComparisonType::LESS_EQUAL, \
            coretest::get_string(first),          \
            coretest::get_string(second)};        \
        coretest::add_assertion(assertion);       \
    }

#define REQUIRE_GREATER(x, y)                  \
    {                                          \
        auto first = x;                        \
        auto second = y;                       \
        coretest::Assertion assertion = {      \
            ((first > second) ? true : false), \
            coretest::AssertionType::REQUIRE,  \
            coretest::ComparisonType::GREATER, \
            coretest::get_string(first),       \
            coretest::get_string(second)};     \
        coretest::add_assertion(assertion);    \
    }

#define REQUIRE_GREATER_EQUAL(x, y)                  \
    {                                                \
        auto first = x;                              \
        auto second = y;                             \
        coretest::Assertion assertion = {            \
            ((first >= second) ? true : false),      \
            coretest::AssertionType::REQUIRE,        \
            coretest::ComparisonType::GREATER_EQUAL, \
            coretest::get_string(first),             \
            coretest::get_string(second)};           \
        coretest::add_assertion(assertion);          \
    }

#define CHECK_EQUAL(x, y)                       \
    {                                           \
        auto first = x;                         \
        auto second = y;                        \
        coretest::Assertion assertion = {       \
            ((first == second) ? true : false), \
            coretest::AssertionType::CHECK,     \
            coretest::ComparisonType::EQUAL,    \
            coretest::get_string(first),        \
            coretest::get_string(second)};      \
        coretest::add_assertion(assertion);     \
    }

#define CHECK_TRUE(x)                         \
    {                                         \
        auto first = x;                       \
        coretest::Assertion assertion = {     \
            ((first == true) ? true : false), \
            coretest::AssertionType::CHECK,   \
            coretest::ComparisonType::TRUE,   \
            coretest::get_string(first)};     \
        coretest::add_assertion(assertion);   \
    }

#define CHECK_FALSE(x)                         \
    {                                          \
        auto first = x;                        \
        coretest::Assertion assertion = {      \
            ((first == false) ? true : false), \
            coretest::AssertionType::CHECK,    \
            coretest::ComparisonType::FALSE,   \
            coretest::get_string(first)};      \
        coretest::add_assertion(assertion);    \
    }

#define CHECK_NOT_EQUAL(x, y)                    \
    {                                            \
        auto first = x;                          \
        auto second = y;                         \
        coretest::Assertion assertion = {        \
            ((first != second) ? true : false),  \
            coretest::AssertionType::CHECK,      \
            coretest::ComparisonType::NOT_EQUAL, \
            coretest::get_string(first),         \
            coretest::get_string(second)};       \
        coretest::add_assertion(assertion);      \
    }

#define CHECK_LESS(x, y)                       \
    {                                          \
        auto first = x;                        \
        auto second = y;                       \
        coretest::Assertion assertion = {      \
            ((first < second) ? true : false), \
            coretest::AssertionType::CHECK,    \
            coretest::ComparisonType::LESS,    \
            coretest::get_string(first),       \
            coretest::get_string(second)};     \
        coretest::add_assertion(assertion);    \
    }

#define CHECK_LESS_EQUAL(x, y)                    \
    {                                             \
        auto first = x;                           \
        auto second = y;                          \
        coretest::Assertion assertion = {         \
            ((first <= second) ? true : false),   \
            coretest::AssertionType::CHECK,       \
            coretest::ComparisonType::LESS_EQUAL, \
            coretest::get_string(first),          \
            coretest::get_string(second)};        \
        coretest::add_assertion(assertion);       \
    }

#define CHECK_GREATER(x, y)                    \
    {                                          \
        auto first = x;                        \
        auto second = y;                       \
        coretest::Assertion assertion = {      \
            ((first > second) ? true : false), \
            coretest::AssertionType::CHECK,    \
            coretest::ComparisonType::GREATER, \
            coretest::get_string(first),       \
            coretest::get_string(second)};     \
        coretest::add_assertion(assertion);    \
    }

#define CHECK_GREATER_EQUAL(x, y)                    \
    {                                                \
        auto first = x;                              \
        auto second = y;                             \
        coretest::Assertion assertion = {            \
            ((first >= second) ? true : false),      \
            coretest::AssertionType::CHECK,          \
            coretest::ComparisonType::GREATER_EQUAL, \
            coretest::get_string(first),             \
            coretest::get_string(second)};           \
        coretest::add_assertion(assertion);          \
    }

#if !defined(CORETEST_IMPLEMENT_WITHOUT_MAIN)
// Standard main entry point
int main(int argc, char **argv) {
    coretest::run_main(argc, argv);
}
#endif
#endif