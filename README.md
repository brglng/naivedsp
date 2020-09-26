# Naive DSP Library

A naive DSP library which implements several audio effects and processing algorithms.

## Build and Install

On Linux and macOS:

    mkdir build
    cd build
    cmake [-DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo>] ..
    make
    sudo make install

On Windows:

    mkdir build
    cd build
    cmake ..
    cmake --build .

## CMake Support

Use `FetchContent`:

    include(FetchContent)
    FetchContent_Declare(naivedsp
        GIT_REPOSITORY    "https://github.com/brglng/naivedsp.git" 
        GIT_SHALLOW       ON
        )
    FetchContent_MakeAvailable(naivedsp)
    add_executable(yourprogram yourprogram.c)
    target_link_libraries(yourprogram naivedsp::common naivedsp::fdn-reverb)

Use `add_subdirectory`:

    add_subdirectory(naivedsp)
    add_executable(yourprogram yourprogram.c)
    target_link_libraries(yourprogram naivedsp::common naivedsp::fdn-reverb)

Use `find_package`:

    find_package(naivedsp)
    add_executable(yourprogram yourprogram.c)
    target_link_libraries(yourprogram naivedsp::common naivedsp::fdn-reverb)
