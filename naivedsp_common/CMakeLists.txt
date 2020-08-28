add_library(naivedsp-common
  src/conversion.c
  src/delay_buf.c
  # src/fir.c
  src/iir_coeffs.c
  src/iir_states.c
  src/iir.c
  src/math.c
  src/memory.c
  )

set_property(TARGET naivedsp-common PROPERTY EXPORT_NAME common)
set_property(TARGET naivedsp-common PROPERTY OUTPUT_NAME naivedsp-common)
set_property(TARGET naivedsp-common PROPERTY INSTALL_RPATH
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
  )

## Building Rules
target_compile_features(naivedsp-common PRIVATE ${naivedsp_compile_features})

target_compile_definitions(naivedsp-common PRIVATE 
  ${naivedsp_compile_definitions}
  )

target_compile_options(naivedsp-common PRIVATE
  ${naivedsp_c_flags}
  $<$<CONFIG:RELEASE>:${naivedsp_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${naivedsp_compile_options_release}>
  )

target_include_directories(naivedsp-common
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  )

target_link_libraries(naivedsp-common
  PUBLIC
    $<$<PLATFORM_ID:Linux>:m>
  )

add_library(naivedsp::common ALIAS naivedsp-common)
export(TARGETS naivedsp-common NAMESPACE naivedsp FILE naivedspTargets.cmake)

install(
  TARGETS naivedsp-common
  EXPORT naivedspTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  install(DIRECTORY include DESTINATION .)
endif()

if(BUILD_TESTING)
  add_subdirectory(tests/iir)
endif()
