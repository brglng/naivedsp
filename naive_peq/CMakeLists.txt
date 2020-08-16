add_library(naive-peq OBJECT
  src/peq.c
  src/peq_coeffs.c
  )

target_include_directories(naive-peq PUBLIC
  include
  ../naivedsp_common/include
  ../naive_peq/include
  )

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
