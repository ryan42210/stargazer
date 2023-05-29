add_library(stargazer-match STATIC
        match.cpp
        coordinate.cpp
        navi_star_filter.cpp)

target_include_directories(stargazer-match PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(stargazer-match PRIVATE Eigen3::Eigen stargazer-math)

