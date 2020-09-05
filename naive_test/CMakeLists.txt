add_library(naive-test
  src/test.c
  )
add_library(naivedsp::test ALIAS naive-test)

set_property(TARGET naive-test PROPERTY EXPORT_NAME test)
set_property(TARGET naive-test PROPERTY OUTPUT_NAME naive-test)
set_property(TARGET naive-test PROPERTY INSTALL_RPATH
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
  )

## Building Rules
target_compile_features(naive-test PRIVATE ${naivedsp_compile_features})

target_compile_definitions(naive-test PRIVATE 
  ${naivedsp_compile_definitions}
  )

target_compile_options(naive-test PRIVATE
  ${naivedsp_c_flags}
  $<$<CONFIG:RELEASE>:${naivedsp_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${naivedsp_compile_options_release}>
  )

target_include_directories(naive-test PUBLIC
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  )

target_link_libraries(naive-test
  PUBLIC
    $<BUILD_INTERFACE:naivedsp-common>
    toml::toml
    wav::wav
    $<$<PLATFORM_ID:Linux>:m>
  )

export(TARGETS naive-test NAMESPACE naivedsp FILE naivedspTargets.cmake)

install(
  TARGETS naive-test
  EXPORT naivedspTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

install(DIRECTORY include DESTINATION .)
