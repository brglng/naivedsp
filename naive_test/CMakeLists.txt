add_library(naive-test OBJECT
  src/test.c
  src/test_desc.c
  )

target_include_directories(naive-test PUBLIC
  include
  ../naivedsp_common/include)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
