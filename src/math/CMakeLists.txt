add_library(stargazer-math STATIC
  array.cpp
)

target_include_directories(stargazer-math PRIVATE
        # ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src/math)


add_executable(math-test math_test.cpp)

target_include_directories(math-test PRIVATE
        ${PROJECT_SOURCE_DIR}/src/math)

target_link_libraries(math-test PRIVATE stargazer-math) 