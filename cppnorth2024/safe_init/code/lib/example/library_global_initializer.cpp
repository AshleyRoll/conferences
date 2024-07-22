#include <fmt/core.h>

#include <example/example_api.hpp>

// Static initialization occurs before main() and as we are creating instances
// of the library_global_initializer in each TU including the core library header,
// all the instances will be initialised during this phase before main().
// All the destruction of these objects will occur after main() exits.
//
// Therefore we can assume single threaded access.
//
// If we were dynamically loading and unloading libraries in some plugin
// architecture (distinct from shared libraries/dlls), we may have to deal
// with thread safety and convert the initializationCounter to a std::atomic<int>.
// However, in this case external libraries may not behave as expected and it
// would be best to only rely on the library Api wrapper for internal initialization.

namespace {
  // The global initialize / finalize methods are places to set up
  // once-per-execution configuration. This is generally needed for
  // external "C" libraries. All internal library initialization
  // should be managed in the library Api wrapper.
  //
  // the global_initialize() and global_finalise() are noexcept,
  // implementation should prevent exceptions propagating, or expect
  // std::terminate() to be called. If this initialization fails it should
  // probably be fatal to the application.
  void global_initialize() noexcept
  {
    // TODO: Call all dependencies start up routines here
    // Example:
    //   libcurl - curl_global_init()
    fmt::println("Initializing lib::example dependencies");
  }

  void global_finalize() noexcept
  {
    // TODO: Call all dependencies clean up routines here
    // Example:
    //   libcurl - curl_global_cleanup()
    fmt::println("Finalizing lib::example dependencies");
  }

  // keep track of the number of initializations
  int initializationCounter;  // static value, zero initialized
}  // namespace

namespace safe_init::lib::example::detail {
  library_global_initializer::library_global_initializer() noexcept
  {
    // if this is the first call, we run the initialization
    if (initializationCounter++ == 0) { global_initialize(); }
  }

  library_global_initializer::~library_global_initializer() noexcept
  {
    // if this is the last call, we run the finalization
    if (--initializationCounter == 0) { global_finalize(); }
  }
}  // namespace safe_init::lib::example::detail
