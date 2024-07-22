#pragma once

#include <string>

#include "library_instance_lock.hpp"
#include "logger.hpp"
#include "queue.hpp"
#include "scoped_singleton.hpp"
#include "worker.hpp"

namespace safe_init::lib::example {

  // The real implementation for the API allowing us to isolate internal implementation details
  // from the library consumers using the PIMPL pattern.
  //
  // This class manages the lifetime of all resources in the example library.
  //
  // Class members are constructed in declaration order and destroyed in reverse.
  // Therefore, we can use the ordering here to control resource lifetimes
  class example_api_impl
  {
  public:
    example_api_impl();
    ~example_api_impl();

    // TODO: Implement public API here
    void do_work(std::string const &message);

    // disable move and copy as we control static resources and should not allow multiple instances
    example_api_impl(example_api_impl &) = delete;
    auto operator=(example_api_impl const &) -> example_api_impl & = delete;
    example_api_impl(example_api_impl &&) = delete;
    auto operator=(example_api_impl &&) -> example_api_impl & = delete;

  private:
    // Members are constructed in declaration order and destroyed in reverse order.
    //
    // We rely on the library_instance_lock to verify a single api instance and this is done before
    // we touch any static data or initialization so that if there is a conflicting instance
    // it will not be affected. This means we need to use a member, not rely on code in the
    // constructor of this class to do the check - it would be too late.
    library_instance_lock m_instanceLock{};

    // our statically accessible log instance. this controls the lifetime of static singleton logger
    // after this object is created, the logger is active and can be used by subsequent components.
    scoped_singleton<logger> m_log;

    // the work queue
    queue m_workQueue{};

    // manages the worker thread and consumes work
    worker m_worker;
  };

}  // namespace safe_init::lib::example