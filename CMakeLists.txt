cmake_minimum_required(VERSION 3.5)

if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/cmake/Modules")

include(naivedspProjectPre)

project(naivedsp C)
set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(PROJECT_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR})
set(${PROJECT_NAME}_VERSION_MINOR 0)
set(PROJECT_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR})
set(${PROJECT_NAME}_VERSION_PATCH 0)
set(PROJECT_VERSION_PATCH ${${PROJECT_NAME}_VERSION_PATCH})
set(${PROJECT_NAME}_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_VERSION ${${PROJECT_NAME}_VERSION})

include(naivedspProjectPost)

add_subdirectory(naivedsp_common)
add_subdirectory(naive_peq)
add_subdirectory(naive_peq_design)
add_subdirectory(naive_geq)
add_subdirectory(naive_geq_design)
add_subdirectory(naive_delay)
add_subdirectory(naive_stereo_delay)
add_subdirectory(naive_reverb)
add_subdirectory(naive_reverb_design)
add_subdirectory(naive_test)

add_library(naivedsp STATIC
  $<TARGET_OBJECTS:naivedsp-common>
  $<TARGET_OBJECTS:naive-delay>
  $<TARGET_OBJECTS:naive-peq>
  $<TARGET_OBJECTS:naive-geq>
  $<TARGET_OBJECTS:naive-stereo-delay>
  $<TARGET_OBJECTS:naive-reverb>
  )

add_library(naivedsp STATIC
  $<TARGET_OBJECTS:naive-peq-design>
  $<TARGET_OBJECTS:naive-geq-design>
  $<TARGET_OBJECTS:naive-reverb-design>
  )
