set(public_headers
  include/naivedsp/delay.h
  )

add_library(naive-delay OBJECT
  src/delay.c
  )

target_include_directories(naive-delay PUBLIC
  include
  ../naivedsp_common/include
  )

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(tests/delay)
