cmake_minimum_required(VERSION 3.25)
project(ProgrammingProject1 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ProgrammingProject1
    src/main.cpp
    src/deck_dealer.cpp
    src/statistics.cpp
)

if(MSVC)
    target_compile_options(ProgrammingProject1 PRIVATE
        /W4
        /EHsc
        /permissive-
        /Zc:__cplusplus
    )
else()
    target_compile_options(ProgrammingProject1 PRIVATE -Wall -Wextra -Wpedantic)
endif()
