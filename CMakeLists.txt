cmake_minimum_required(VERSION 3.14)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()
if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

# Project Information
project(naivedsp C)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(${PROJECT_NAME}_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(${PROJECT_NAME}_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(${PROJECT_NAME}_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(${PROJECT_NAME}_VERSION ${PROJECT_VERSION})

# CMake variables that affects building
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
  endif()

  if(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "enable building of shared libraries instead of static ones" FORCE)
  endif()

  if(NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "enable position independent code" FORCE)
  endif()
endif()

# Custom build options
option(NAIVEDSP_ENABLE_OPT_CODE "enable platform optimized code" ON)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  include(CTest)
  message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
  message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
  message(STATUS "CMAKE_POSITION_INDEPENDENT_CODE: ${CMAKE_POSITION_INDEPENDENT_CODE}")
  message(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
  message(STATUS "BUILD_TESTING: ${BUILD_TESTING}")
endif()

message(STATUS "NAIVEDSP_ENABLE_OPT_CODE: ${NAIVEDSP_ENABLE_OPT_CODE}")

set(naivedsp_full_configuration "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE} -DNAIVEDSP_ENABLE_OPT_CODE=${NAIVEDSP_ENABLE_OPT_CODE}")

include(GNUInstallDirs)
include(naivedspTargetProperties)

add_subdirectory(deps)
add_subdirectory(naivedsp_common)
# add_subdirectory(naive_peq)
# add_subdirectory(naive_linkwitz_riley_geq)
# add_subdirectory(naive_cascading_geq)
# add_subdirectory(naive_fir_geq)
# add_subdirectory(naive_delay)
# add_subdirectory(naive_stereo_delay)
# add_subdirectory(naive_fdn_reverb)
add_subdirectory(naive_test)

export(PACKAGE naivedsp)

# Install README and CHANGELOG
install(
  FILES README.md CHANGELOG.md
  DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/${PROJECT_NAME}
  )

install(EXPORT naivedspTargets
  FILE naivedspTargets.cmake
  NAMESPACE naivedsp::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  naivedspConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY SameMajorVersion
  )

configure_file(cmake/naivedspConfig.cmake.in naivedspConfig.cmake @ONLY)
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/naivedspConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/naivedspConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  )
  
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  set(CPACK_PACKAGE_NAME "naivedsp")
  set(CPACK_GENERATOR "TXZ")
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
  set(CPACK_SOURCE_IGNORE_FILES "/\\\\.git/;\\\\.git.*;/build/;/backup/;/cmake-build-.*/;/\\\\.idea/;/\\\\.ycm_extra_conf\\\\..*;/GPATH$;/GRTAGS$;/GSYMS$;/GTAGS$;\\\\.swp$;\\\\.swo$;.DS_Store;.ccls;.ccls-cache")
  set(CPACK_SOURCE_GENERATOR "TXZ")
  include(CPack)
endif()
