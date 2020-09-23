add_library(naive-stereo-limiter src/stereo_limiter.c)
add_library(naivedsp::stereo-limiter ALIAS naive-stereo-limiter)

set_property(TARGET naive-stereo-limiter PROPERTY EXPORT_NAME stereo-limiter)
set_property(TARGET naive-stereo-limiter PROPERTY OUTPUT_NAME naive-stereo-limiter)
set_property(TARGET naive-stereo-limiter PROPERTY INSTALL_RPATH
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
  )

## Building Rules
target_compile_features(naive-stereo-limiter PRIVATE ${naivedsp_compile_features})

target_compile_definitions(naive-stereo-limiter PRIVATE 
  ${naivedsp_compile_definitions}
  )

target_compile_options(naive-stereo-limiter PRIVATE
  ${naivedsp_c_flags}
  $<$<CONFIG:RELEASE>:${naivedsp_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${naivedsp_compile_options_release}>
  )

target_include_directories(naive-stereo-limiter
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  )

target_link_libraries(naive-stereo-limiter
  PUBLIC
    naivedsp::common
    $<$<PLATFORM_ID:Linux>:m>
  )

export(TARGETS naive-stereo-limiter NAMESPACE naivedsp FILE naivedspTargets.cmake)

install(
  TARGETS naive-stereo-limiter
  EXPORT naivedspTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

install(DIRECTORY include DESTINATION .)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} AND BUILD_TESTING)
  add_subdirectory(tests/stereo_limiter)
endif()
