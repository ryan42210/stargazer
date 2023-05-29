add_library(stargazer-solver STATIC
        coordinate.cpp
        navi_star_filter.cpp
        math_util.cpp)


target_include_directories(stargazer-solver PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(stargazer-solver PRIVATE Eigen3::Eigen)

if(OpenMP_CXX_FOUND)
    target_link_libraries(stargazer-solver PRIVATE OpenMP::OpenMP_CXX)
endif()