#pragma once

#include <atomic>

#include <example/exceptions.hpp>

namespace safe_init::lib::example {

  struct library_instance_lock
  {
    // Rely on default initialization which will initialise this during the zero-init phase
    // and so be false at the time the program starts executing.
    constinit inline static std::atomic_bool s_instanceAlive{};

    library_instance_lock()
    {
      bool expected{ false };
      if (not s_instanceAlive.compare_exchange_strong(expected, true)) {
        // the previous value was true, and so there is already an instance of the api
        throw library_instance_already_exists();
      }
    }

    ~library_instance_lock()
    {
      // We are the only instance existing, so we can just write a false atomically
      s_instanceAlive.store(false);
    }

    // disable move and copy as we want there to be a single instance
    // managing the lifetime of the api through the static instanceAlive member.
    library_instance_lock(library_instance_lock &) = delete;
    auto operator=(library_instance_lock const &) -> library_instance_lock & = delete;
    library_instance_lock(library_instance_lock &&) = delete;
    auto operator=(library_instance_lock &&) -> library_instance_lock & = delete;
  };

}  // namespace safe_init::lib::example