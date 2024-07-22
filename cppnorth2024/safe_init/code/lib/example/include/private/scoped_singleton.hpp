#pragma once

#include <stdexcept>
#include <utility>

namespace safe_init::lib::example {

  template<typename T>
  class scoped_singleton
  {
  public:
    template<typename... Args>
    explicit scoped_singleton(Args &&...args)
      : m_instance(std::forward<Args>(args)...)
    {
      // verify that no other instance is active, we have already created our instance,
      // but this should already be guarded by the owning API instance lock, so this is a sanity
      // check and will only fail because of programming errors in the library attempting to create
      // multiple instances
      if (s_instance != nullptr) {
        throw std::logic_error(
          "An instance of the scoped_singleton<T> is already active. Only a single instance at a time is permitted");
      }
      s_instance = &m_instance;
    }

    ~scoped_singleton() { s_instance = nullptr; }

    // disable move and copy as we control static resources and should not allow multiple instances
    scoped_singleton(scoped_singleton &) = delete;
    auto operator=(scoped_singleton const &) -> scoped_singleton & = delete;
    scoped_singleton(scoped_singleton &&) = delete;
    auto operator=(scoped_singleton &&) -> scoped_singleton & = delete;

    [[maybe_unused, nodiscard]] static auto instance() -> T &
    {
      // Validate that the instance is actually live.
      if (s_instance == nullptr) {
        throw std::logic_error("scoped_singleton<T>::instance() called without live instance");
      }
      return *s_instance;
    }

  private:
    T m_instance;
    // define the static instance pointer and set to null pointer
    // as this will happen in the zero-init phase before code using it runs
    constinit inline static T *s_instance{nullptr};
  };

}  // namespace safe_init::lib::example