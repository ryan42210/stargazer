add_library(stargazer-io STATIC
  ${CMAKE_CURRENT_LIST_DIR}/catalogue_io.cpp
  ${CMAKE_CURRENT_LIST_DIR}/catalogue_io.h
)

if(OpenMP_CXX_FOUND)
  target_link_libraries(stargazer-io PUBLIC OpenMP::OpenMP_CXX)
endif()