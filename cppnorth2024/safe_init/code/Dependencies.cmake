include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(safe_init_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project

  if(NOT TARGET fmtlib::fmtlib)
    CPMAddPackage("gh:fmtlib/fmt#10.2.1")
  endif()

#  if(NOT TARGET Catch2::Catch2WithMain)
#    CPMAddPackage("gh:catchorg/Catch2@3.5.3")
#  endif()
endfunction()
