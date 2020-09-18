add_library(naive-fdn-reverb
  src/fdn_reverb.c
  src/early_reflection.c
  )

add_library(naivedsp::fdn-reverb ALIAS naive-fdn-reverb)

set_property(TARGET naive-fdn-reverb PROPERTY EXPORT_NAME fdn-reverb)
set_property(TARGET naive-fdn-reverb PROPERTY OUTPUT_NAME naive-fdn-reverb)
set_property(TARGET naive-fdn-reverb PROPERTY INSTALL_RPATH
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
  )

## Building Rules
target_compile_features(naive-fdn-reverb PRIVATE ${naivedsp_compile_features})

target_compile_definitions(naive-fdn-reverb PRIVATE 
  ${naivedsp_compile_definitions}
  )

target_compile_options(naive-fdn-reverb PRIVATE
  ${naivedsp_c_flags}
  $<$<CONFIG:RELEASE>:${naivedsp_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${naivedsp_compile_options_release}>
  )

target_include_directories(naive-fdn-reverb
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  )

target_link_libraries(naive-fdn-reverb
  PUBLIC
    naivedsp::common
    $<$<PLATFORM_ID:Linux>:m>
  )

export(TARGETS naive-fdn-reverb NAMESPACE naivedsp FILE naivedspTargets.cmake)

install(
  TARGETS naive-fdn-reverb
  EXPORT naivedspTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

install(DIRECTORY include DESTINATION .)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} AND BUILD_TESTING)
  add_subdirectory(tests/fdn_reverb)
endif()
