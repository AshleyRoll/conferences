cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
        safe_init
        VERSION 0.1.0.0
        LANGUAGES CXX
)


include(cmake/PreventInSourceBuilds.cmake)
include(cmake/GitHash.cmake)
include(ProjectOptions.cmake)

safe_init_setup_options()

include(Dependencies.cmake)
safe_init_setup_dependencies()

safe_init_local_options()

# don't know if this should be set globally from here or not...
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

target_compile_features(safe_init_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_library(safe_init::safe_init_options ALIAS safe_init_options)
add_library(safe_init::safe_init_warnings ALIAS safe_init_warnings)


# Add in the various targets
add_subdirectory(lib)
add_subdirectory(src)
