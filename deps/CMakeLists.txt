set(FETCHCONTENT_QUIET OFF)
include(FetchContent)
FetchContent_Declare(libtoml
  GIT_REPOSITORY    "https://github.com/brglng/libtoml.git" 
  GIT_SHALLOW       ON
  )
FetchContent_MakeAvailable(libtoml)
FetchContent_Declare(libwav
  GIT_REPOSITORY    "https://github.com/brglng/libwav.git" 
  GIT_SHALLOW       ON
  )
FetchContent_MakeAvailable(libwav)
