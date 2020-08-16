add_library(naivedsp-common OBJECT
  src/conversion.c
  src/delay_buf.c
  src/iir.c
  src/iir_coeffs.c
  src/iir_design.c
  src/math.c
  src/memory.c
  )

target_include_directories(naivedsp-common PUBLIC include)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(tests/iir)
