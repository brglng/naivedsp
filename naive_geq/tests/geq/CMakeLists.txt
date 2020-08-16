add_executable(naive-geq-test
  main.c
  test_desc.c
  $<TARGET_OBJECTS:naivedsp-common>
  $<TARGET_OBJECTS:naive-peq>
  $<TARGET_OBJECTS:naive-peq-design>
  $<TARGET_OBJECTS:naive-geq>
  $<TARGET_OBJECTS:naive-geq-design>
  $<TARGET_OBJECTS:naive-test>
  )

target_include_directories(naive-geq-test PUBLIC
  ../../include
  ../../../naive_test/include
  ../../../naive_geq_design/include
  ../../../naivedsp_common/include
  ../../../naive_peq/include
  ../../../naive_peq_design/include
  )

target_compile_definitions(naive-geq-test PRIVATE
  NAIVE_TEST_REFS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/refs"
  )

if (("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"))
  target_link_libraries(naive-geq-test m)
endif()

