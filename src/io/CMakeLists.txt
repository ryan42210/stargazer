add_library(stargazer-io STATIC
  catalogue_io.cpp
  navi_star_list_io.cpp
  cam_input.cpp
)
target_include_directories(stargazer-io PUBLIC
        ${PROJECT_SOURCE_DIR}/src/io
        ${PROJECT_SOURCE_DIR}/include)


add_executable(io-test io_test.cpp)

target_link_libraries(io-test PRIVATE stargazer-io)

target_include_directories(io-test PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src/io)
