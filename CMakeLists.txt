cmake_minimum_required(VERSION 3.20)

project(stargazer)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/bigobj)
endif(MSVC)

find_package(OpenMP REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

add_subdirectory(src)