option(CMAKE_BUILD_TYPE                 "build type"                                            Release)
option(BUILD_SHARED_LIBS                "build shared libraries instead of static libraries"    OFF)
option(CMAKE_POSITION_INDEPENDENT_CODE  "build position independent code"                       OFF)
option(ENABLE_TESTING                   "enable testing"                                        ON)
option(ENABLE_OPT_CODE                  "enable platform optimized code"                        ON)

if(ENABLE_TESTING)
  enable_testing()
endif()

add_definitions(-D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -DNAIVE_TEST_WORKING_DIR="${PROJECT_BINARY_DIR}" -DNAIVE_TEST_INPUTS_DIR="${PROJECT_SOURCE_DIR}/naive_test/inputs/")

if(ENABLE_OPT_CODE)
  add_definitions(-DNAIVE_ENABLE_OPT_CODE=1)
else()
  add_definitions(-DNAIVE_ENABLE_OPT_CODE=0)
endif()

if(${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall /wd4819 /wd4996 /wd4820 /wd4710")
elseif(${CMAKE_C_COMPILER_ID} MATCHES "^(GNU|.*Clang)$")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror=implicit -Wno-shift-negative-value -std=c99")

  if((${CMAKE_C_COMPILER_ID} STREQUAL "GNU") AND (${CMAKE_C_COMPILER_VERSION} VERSION_GREATER "6.0.0"))
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=discarded-qualifiers -Werror=incompatible-pointer-types")
  endif()

  if((${CMAKE_C_COMPILER_ID} MATCHES ".*Clang$"))
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=ignored-qualifiers -Werror=incompatible-pointer-types")
  endif()

  if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^x86")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fomit-frame-pointer -march=native")
  endif()
endif()

set(CMAKE_ASM_ADSP_SOURCE_FILE_EXTENSIONS asm s S CACHE LIST "")
set(CMAKE_ASM_ADSP_OUTPUT_EXTENSION ".doj" CACHE STRING "")
set(CMAKE_ASM_ADSP_COMPILE_OBJECT
  "<CMAKE_ASM_COMPILER> <INCLUDES> <FLAGS> -proc ${ADSP_PROCESSOR} -si-revision ${ADSP_PROCESSOR_SILICIUM_REVISION} -o <OBJECT> <SOURCE>" CACHE STRING "")

set(CMAKE_C_EXTENSIONS      OFF)
set(CMAKE_C_STANDARD        99)

include(GNUInstallDirs)

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_SOURCE_IGNORE_FILES "/\\\\.git/;\\\\.git.*;/build/;/\\\\.idea/;/\\\\.ycm_extra_conf\\\\..*;/GPATH$;/GRTAGS$;/GSYMS$;/GTAGS$;\\\\.swp$;\\\\.swo$")
set(CPACK_SOURCE_GENERATOR "ZIP")
include(CPack)
