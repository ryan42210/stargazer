add_library(stargazer-math STATIC
    coordinate.cpp)

target_link_libraries(stargazer-math PRIVATE Eigen3::Eigen)

target_include_directories(stargazer-math PRIVATE
        ${PROJECT_SOURCE_DIR}/include)