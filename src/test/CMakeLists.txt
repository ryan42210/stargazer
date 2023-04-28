add_executable(catalogue_io-test catalogue_io_demo.cpp)

target_link_libraries(catalogue_io-test PRIVATE stargazer-io)

target_include_directories(catalogue_io-test PRIVATE ${PROJECT_SOURCE_DIR}/include)