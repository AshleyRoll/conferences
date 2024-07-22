include(cmake/SystemLink.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(safe_init_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(safe_init_setup_options)
  option(safe_init_ENABLE_COVERAGE "Enable coverage reporting" OFF)

  safe_init_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR safe_init_PACKAGING_MAINTAINER_MODE)
    option(safe_init_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(safe_init_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(safe_init_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(safe_init_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(safe_init_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(safe_init_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(safe_init_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(safe_init_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(safe_init_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(safe_init_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(safe_init_ENABLE_CACHE "Enable ccache" OFF)
  else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      # debug builds
      option(safe_init_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
      option(safe_init_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
      option(safe_init_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
      option(safe_init_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
      option(safe_init_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
      option(safe_init_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
      option(safe_init_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
      option(safe_init_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
      option(safe_init_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
      option(safe_init_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
      option(safe_init_ENABLE_CACHE "Enable ccache" ON)
    else()
      # release builds
      option(safe_init_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
      option(safe_init_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
      option(safe_init_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
      option(safe_init_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
      option(safe_init_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
      option(safe_init_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
      option(safe_init_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
      option(safe_init_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
      option(safe_init_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
      option(safe_init_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
      option(safe_init_ENABLE_CACHE "Enable ccache" OFF)
    endif()
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      safe_init_WARNINGS_AS_ERRORS
      safe_init_ENABLE_USER_LINKER
      safe_init_ENABLE_SANITIZER_ADDRESS
      safe_init_ENABLE_SANITIZER_LEAK
      safe_init_ENABLE_SANITIZER_UNDEFINED
      safe_init_ENABLE_SANITIZER_THREAD
      safe_init_ENABLE_SANITIZER_MEMORY
      safe_init_ENABLE_UNITY_BUILD
      safe_init_ENABLE_CLANG_TIDY
      safe_init_ENABLE_CPPCHECK
      safe_init_ENABLE_COVERAGE
      safe_init_ENABLE_CACHE)
  endif()
endmacro()

macro(safe_init_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(safe_init_warnings INTERFACE)
  add_library(safe_init_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  safe_init_set_project_warnings(
    safe_init_warnings
    ${safe_init_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(safe_init_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(safe_init_options)
  endif()

  include(cmake/Sanitizers.cmake)
  safe_init_enable_sanitizers(
    safe_init_options
    ${safe_init_ENABLE_SANITIZER_ADDRESS}
    ${safe_init_ENABLE_SANITIZER_LEAK}
    ${safe_init_ENABLE_SANITIZER_UNDEFINED}
    ${safe_init_ENABLE_SANITIZER_THREAD}
    ${safe_init_ENABLE_SANITIZER_MEMORY})

  set_target_properties(safe_init_options PROPERTIES UNITY_BUILD ${safe_init_ENABLE_UNITY_BUILD})

  if(safe_init_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    safe_init_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(safe_init_ENABLE_CLANG_TIDY)
    safe_init_enable_clang_tidy(safe_init_options ${safe_init_WARNINGS_AS_ERRORS})
  endif()

  if(safe_init_ENABLE_CPPCHECK)
    safe_init_enable_cppcheck(${safe_init_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(safe_init_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    safe_init_enable_coverage(safe_init_options)
  endif()

  if(safe_init_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(safe_init_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()
endmacro()
