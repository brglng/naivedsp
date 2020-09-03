add_library(naive-peq src/peq.c)
add_library(naivedsp::peq ALIAS naive-peq)

set_property(TARGET naive-peq PROPERTY EXPORT_NAME peq)
set_property(TARGET naive-peq PROPERTY OUTPUT_NAME naive-peq)
set_property(TARGET naive-peq PROPERTY INSTALL_RPATH
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
  "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
  )

## Building Rules
target_compile_features(naive-peq PRIVATE ${naivedsp_compile_features})

target_compile_definitions(naive-peq PRIVATE 
  ${naivedsp_compile_definitions}
  )

target_compile_options(naive-peq PRIVATE
  ${naivedsp_c_flags}
  $<$<CONFIG:RELEASE>:${naivedsp_compile_options_release}>
  $<$<CONFIG:RELWITHDEBINFO>:${naivedsp_compile_options_release}>
  )

target_include_directories(naive-peq
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  )

target_link_libraries(naive-peq
  PUBLIC
    naivedsp::common
    $<$<PLATFORM_ID:Linux>:m>
  )

export(TARGETS naive-peq NAMESPACE naivedsp FILE naivedspTargets.cmake)

install(
  TARGETS naive-peq
  EXPORT naivedspTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  install(DIRECTORY include DESTINATION .)
endif()
