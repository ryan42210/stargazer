add_subdirectory(v_cam)
add_subdirectory(io)
add_subdirectory(solver)
add_subdirectory(test)

add_executable(${PROJECT_NAME}-simulator simulator.cpp)

target_link_libraries(${PROJECT_NAME}-simulator PRIVATE
        Eigen3::Eigen
        stargazer-match
        stargazer-math
        stargazer-io)

target_include_directories(${PROJECT_NAME}-simulator PRIVATE ${PROJECT_SOURCE_DIR}/include)
