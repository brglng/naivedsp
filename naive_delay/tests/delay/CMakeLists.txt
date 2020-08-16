add_executable(naive-delay-test
  main.c
  test_desc.c
  $<TARGET_OBJECTS:naive-delay>
  $<TARGET_OBJECTS:naivedsp-common>
  $<TARGET_OBJECTS:naive-test>
  )

target_compile_definitions(naive-delay-test PRIVATE
  NAIVE_TEST_REFS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/refs"
  )

target_include_directories(naive-delay-test PUBLIC
  ../../include
  ../../../naivedsp_common/include
  ../../../naive_test/include
  )

if (("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"))
  target_link_libraries(naive-delay-test m)
endif()
