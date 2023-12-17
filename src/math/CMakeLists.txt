add_library(stargazer-math STATIC
  array.cpp
)

target_include_directories(stargazer-math PRIVATE
        ${PROJECT_SOURCE_DIR}/include)