cmake_minimum_required(VERSION 3.6)
project(LetterCounter)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(LetterCounter ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(LetterCounter pthread)
