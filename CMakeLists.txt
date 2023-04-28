cmake_minimum_required(VERSION 3.20)

project(stargazer)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

add_subdirectory(src)