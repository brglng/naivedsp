add_library(naive-peq-design OBJECT
  src/peq_design.c
  )

target_include_directories(naive-peq-design PUBLIC
  include
  ../naivedsp_common/include
  ../naive_common_design/include
  ../naive_peq/include
  )

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
