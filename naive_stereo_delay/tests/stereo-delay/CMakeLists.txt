link_directories(${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

add_executable(naive-stereo-delay-test main.c $<TARGET_OBJECTS:naive-stereo-delay> $<TARGET_OBJECTS:naivedsp-common>)

if (("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"))
  target_link_libraries(naive-stereo-delay-test m)
endif()

target_include_directories(naive-stereo-delay-test PUBLIC
  ../../include
  ../../../naivedsp-common/include)

install(TARGETS naive-stereo-delay-test DESTINATION ${CMAKE_INSTALL_BINDIR})
