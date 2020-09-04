add_library(naive-delay src/delay.c)
add_library(naivedsp::delay ALIAS naive-delay)

set_property(TARGET naive-delay PROPERTY EXPORT_NAME delay)
set_property(TARGET naive-delay PROPERTY OUTPUT_NAME naive-delay)
set_property(TARGET naive-delay PROPERTY INSTALL_RPATH
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
  )

## Building Rules
target_compile_features(naive-delay PRIVATE ${naivedsp_compile_features})

target_compile_definitions(naive-delay PRIVATE 
  ${naivedsp_compile_definitions}
  )

target_compile_options(naive-delay PRIVATE
  ${naivedsp_c_flags}
  $<$<CONFIG:RELEASE>:${naivedsp_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${naivedsp_compile_options_release}>
  )

target_include_directories(naive-delay
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  )

target_link_libraries(naive-delay
  PUBLIC
    naivedsp::common
    $<$<PLATFORM_ID:Linux>:m>
  )

export(TARGETS naive-delay NAMESPACE naivedsp FILE naivedspTargets.cmake)

install(
  TARGETS naive-delay
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
  add_subdirectory(tests/delay)
endif()
