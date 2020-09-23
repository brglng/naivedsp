# Naive DSP Library

A naive DSP library which implements several audio effects and processing algorithms.

## Build

    mkdir build
    cd build
    cmake [-DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo>] ..
    make

## CMake Support

Use in a build tree:

    # Use FetchContent
    include(FetchContent)
    FetchContent_Declare(naivedsp
        GIT_REPOSITORY    "https://github.com/brglng/naivedsp.git" 
        GIT_SHALLOW       ON
        )
    FetchContent_MakeAvailable(naivedsp)

    # Or add_subdirectory manually
    # add_subdirectory(naivedsp)      # placed in your project tree

    add_executable(yourprogram yourprogram.c)
    target_link_libraries(yourprogram PRIVATE naivedsp::common naivedsp::fdn-reverb)

Use with `find_package`

    find_package(naivedsp)
    add_executable(yourprogram yourprogram.c)
    target_link_libraries(yourprogram PRIVATE naivedsp::common naivedsp::fdn-reverb)
