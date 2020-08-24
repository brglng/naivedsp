add_executable(naive-iir-test
  main.c
  test_desc.c
  )

target_compile_features(naivedsp PRIVATE ${naivedsp_compile_features})

target_compile_definitions(naivedsp PRIVATE
  ${naivedsp_compile_definitions}
  ${naivedsp_test_compile_definitions}
  )

target_compile_options(naivedsp PRIVATE
  ${naivedsp_c_flags}
  $<$<CONFIG:RELEASE>:${naivedsp_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${naivedsp_compile_options_release}>
  )

target_link_libraries(naivedsp
  naivedsp-common
  naive-test
  ${naivedsp_link_libraries})
