add_library(naive-reverb-design OBJECT
  src/reverb_design.c
  )

target_include_directories(naive-reverb-design
  PUBLIC
    include
    ../naivedsp_common/include
    ../naive_common_design/include
    ../naive_peq/include
    ../naive_peq_design/include
    ../naive_geq/include
    ../naive_geq_design/include
    ../naive_reverb/include
  )

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
