add_library(stargazer-io STATIC
  catalogue_io.cpp
  navi_star_list_io.cpp
  cam_input.cpp
)

target_include_directories(stargazer-io PRIVATE
        ${PROJECT_SOURCE_DIR}/include)

if(OpenMP_CXX_FOUND)
  target_link_libraries(stargazer-io PUBLIC OpenMP::OpenMP_CXX)
endif()