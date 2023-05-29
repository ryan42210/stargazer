add_executable(io-test io_test.cpp)
target_link_libraries(io-test PRIVATE stargazer-io)
target_include_directories(io-test PRIVATE ${PROJECT_SOURCE_DIR}/include)


add_library(stargazer-test STATIC test.cpp)
target_link_libraries(stargazer-test PRIVATE Eigen3::Eigen)
target_include_directories(stargazer-test PRIVATE
        ${PROJECT_SOURCE_DIR}/include)