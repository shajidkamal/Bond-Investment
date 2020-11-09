cmake_minimum_required(VERSION 3.12)
project(Project2)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(InputReader)
add_executable(Project2 main.cpp StoreInput.cpp StoreInput.hpp Date.cpp Date.hpp Algorithm.cpp Algorithm.hpp Strategy.cpp Strategy.hpp)
target_link_libraries(Project2 PRIVATE InputReader)