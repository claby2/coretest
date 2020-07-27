project(core-test-tests CXX)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

macro(test _name)
    add_executable(${_name} ${_name}.cpp)
endmacro()

test(test)