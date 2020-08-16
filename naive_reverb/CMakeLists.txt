add_library(naive-reverb OBJECT
  src/reverb.c
  src/reverb_params.c
  )

target_include_directories(naive-reverb
  PUBLIC
    include
    ../naivedsp_common/include
    ../naive_peq/include
  )

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(tests/reverb)
