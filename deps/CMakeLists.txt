set(FETCHCONTENT_QUIET OFF)
include(FetchContent)
FetchContent_Declare(libtoml
  GIT_REPOSITORY    "https://github.com/brglng/libtoml.git" 
  GIT_SHALLOW       ON
  )
FetchContent_MakeAvailable(libtoml)
