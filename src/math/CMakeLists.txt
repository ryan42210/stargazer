add_library(stargazer-math STATIC
  matrix.cpp
)

target_include_directories(stargazer-math PRIVATE
        ${PROJECT_SOURCE_DIR}/include)

if(OpenMP_CXX_FOUND)
  target_link_libraries(stargazer-io PRIVATE OpenMP::OpenMP_CXX)
endif()