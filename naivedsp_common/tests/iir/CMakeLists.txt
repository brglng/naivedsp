add_executable(naive-iir-test
  main.c
  test_desc.c
  $<TARGET_OBJECTS:naivedsp-common>
  $<TARGET_OBJECTS:naivedsp-common-design>
  $<TARGET_OBJECTS:naive-test>
  )

target_include_directories(naive-iir-test PUBLIC
  ../../include
  ../../../naive_common_design/include
  ../../../naive_test/include
  )

target_compile_definitions(naive-iir-test PRIVATE
  NAIVE_TEST_REFS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/refs"
  )

if (("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"))
  target_link_libraries(naive-iir-test m)
endif()
