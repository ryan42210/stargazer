
add_library(stargazer-test STATIC image_generator.cpp)
target_link_libraries(stargazer-test PRIVATE stargazer-solver)
target_include_directories(stargazer-test PRIVATE
        ${PROJECT_SOURCE_DIR}/include)