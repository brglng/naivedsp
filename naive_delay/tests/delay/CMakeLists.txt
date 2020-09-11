add_executable(naive-delay-test
  main.c
  )

target_compile_features(naive-delay-test PRIVATE ${naivedsp_compile_features})

target_compile_definitions(naive-delay-test PRIVATE
  ${naivedsp_compile_definitions}
  NAIVE_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
  NAIVE_TEST_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}"
  NAIVE_TEST_INPUTS_DIR="${PROJECT_SOURCE_DIR}/naive_test/inputs"
  )

target_compile_options(naive-delay-test PRIVATE
  ${naivedsp_c_flags}
  $<$<CONFIG:RELEASE>:${naivedsp_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${naivedsp_compile_options_release}>
  )

target_link_libraries(naive-delay-test
  naivedsp::common
  naivedsp::delay
  naivedsp::test
  ${naivedsp_link_libraries})

add_custom_command(
  TARGET naive-delay-test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/outputs)

add_test(NAME naive-delay-test COMMAND $<TARGET_FILE:naive-delay-test>)
