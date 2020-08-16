add_library(naive-geq OBJECT
  src/geq.c
  src/geq_coeffs.c
  )

target_include_directories(naive-geq PUBLIC
  include
  ../naivedsp_common/include
  ../naive_peq/include
  )

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(tests/geq)
