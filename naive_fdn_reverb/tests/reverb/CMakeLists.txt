add_executable(naive-reverb-test
  main.c
  $<TARGET_OBJECTS:naivedsp-common>
  $<TARGET_OBJECTS:naivedsp-common-design>
  $<TARGET_OBJECTS:naive-peq>
  $<TARGET_OBJECTS:naive-peq-design>
  $<TARGET_OBJECTS:naive-geq>
  $<TARGET_OBJECTS:naive-geq-design>
  $<TARGET_OBJECTS:naive-reverb>
  $<TARGET_OBJECTS:naive-reverb-design>
  $<TARGET_OBJECTS:naive-test>
  )

target_include_directories(naive-reverb-test
  PRIVATE
    ../../include
    ../../../naivedsp_common/include
    ../../../naive_common_design/include
    ../../../naive_peq/include
    ../../../naive_peq_design/include
    ../../../naive_geq/include
    ../../../naive_geq_design/include
    ../../../naive_reverb_design/include
    ../../../naive_test/include
  )

target_compile_definitions(naive-reverb-test
  PRIVATE
    NAIVE_TEST_REFS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/refs"
  )

if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_C_COMPILER_ID} STREQUAL "Clang" OR
    ${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang")
  target_link_libraries(naive-reverb-test m)
endif()

# add_custom_command(
#   TARGET naive-reverb-test POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/cmake/CopyIfNotExists.cmake
#     ${CMAKE_CURRENT_SOURCE_DIR}/naive-reverb-test.ttd
#     ${CMAKE_BINARY_DIR}
#     )

# add_custom_command(
#   TARGET naive-reverb-test POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/test-out)

install(TARGETS naive-reverb-test DESTINATION ${CMAKE_INSTALL_BINDIR})
