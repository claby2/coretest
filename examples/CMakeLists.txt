project(core-test-examples CXX)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

macro(example _name)
    add_executable(${_name} ${_name}.cpp)
endmacro()

example(tutorial_sections)
example(tutorial_tests)