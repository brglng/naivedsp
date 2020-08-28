set(naivedsp_compile_features
  c_variadic_macros
  )

set(naivedsp_compile_definitions
  __STDC_FORMAT_MACROS
  __STDC_LIMIT_MACROS
  __STDC_CONSTANT_MACROS
  )

if(NAIVEDSP_ENABLE_OPT_CODE)
  list(APPEND naivedsp_compile_definitions NAIVE_ENABLE_OPT_CODE=1)
else()
  list(APPEND naivedsp_compile_definitions NAIVE_ENABLE_OPT_CODE=0)
endif()

set(naivedsp_c_flags "")
include(CheckCCompilerFlag)
if(${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")
elseif(${CMAKE_C_COMPILER_ID} MATCHES "^(GNU|.*Clang)$")
  foreach(flag -fno-strict-aliasing
               -Wall
               -Wcast-align
               -Wduplicated-branches
               -Wduplicated-cond
               -Wextra
               -Wformat=2
               -Wmissing-include-dirs
               -Wsign-conversion
               -Wnarrowing
               -Wpointer-arith
               -Wshadow
               -Wuninitialized
               -Wwrite-strings
               -Werror=discarded-qualifiers
               -Werror=ignored-qualifiers
               -Werror=implicit
               -Werror=implicit-function-declaration
               -Werror=implicit-int
               -Werror=init-self
               -Werror=incompatible-pointer-types
               -Werror=return-type
               -Werror=strict-prototypes
               )
    check_c_compiler_flag(${flag} naivedsp_has_c_flag_${flag})
    if(naivedsp_has_c_flag_${flag})
      list(APPEND naivedsp_c_flags ${flag})
    endif()
  endforeach()
endif()

set(naivedsp_compile_options_release -fomit-frame-pointer -march=native -mtune=native)
