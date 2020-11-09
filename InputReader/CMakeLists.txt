cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 14)
add_library(InputReader STATIC    CSVInputReader.cpp    CSVInputReader.hpp    FixedWidthInputReader.hpp    InputField.cpp    InputField.hpp    InputReader.cpp    InputReader.hpp    TSVInputReader.cpp    TSVInputReader.hpp)
target_include_directories(InputReader    PUBLIC ${CMAKE_CURRENT_LIST_DIR})
