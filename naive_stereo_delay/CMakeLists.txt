set(public_headers
  include/naivedsp/stereo-delay.h
  )

add_library(naive-stereo-delay OBJECT
  src/stereo-delay.c
  )

target_include_directories(naive-stereo-delay PUBLIC
  include
  ../naivedsp-common/include)

install(TARGETS naive-stereo-delay DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS naive-stereo-delay DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# add_subdirectory(tests/stereo-delay)
