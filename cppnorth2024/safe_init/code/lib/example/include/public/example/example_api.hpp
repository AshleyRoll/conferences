#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <example/example_export.hpp>
#include <example/version.hpp>

namespace safe_init::lib::example {

  namespace detail {
    // This struct is instantiated in every translation unit that references
    // the example library. Its job is to ensure the library has a chance at
    // performing global start up and shutdown operations such as initialising
    // its dependencies and libraries. It does this through a static counter
    // tracking the number of instances of this class.
    //
    // See Also 'Nifty Counter' idiom
    [[maybe_unused]] static struct EXAMPLE_EXPORT library_global_initializer
    {
      library_global_initializer() noexcept;
      ~library_global_initializer() noexcept;

      // prevent copy and move to avoid corrupting internal counter state.
      // The client application should not be manipulating this object anyway.
      library_global_initializer(library_global_initializer &) = delete;
      auto operator=(library_global_initializer const &) -> library_global_initializer & = delete;
      library_global_initializer(library_global_initializer &&) = delete;
      auto operator=(library_global_initializer &&) -> library_global_initializer & = delete;
    } const LibraryGlobalInitializer;  // instance per TU using this include file
  }  // namespace detail

  // forward declare PIMPL abstraction for example_api
  class example_api_impl;

  // The public interface to the example library.
  //
  // The client application must:
  // - create and hold an instance while it wishes to use the library
  // - may create and destroy multiple instances, as long as lifetimes do not overlap.
  // - must ensure that the instance is destroyed before the application exits
  class EXAMPLE_EXPORT example_api
  {
  public:
    example_api()
      : example_api(expected_version{})
    {}
    ~example_api();

    // disable copy and only allow move for the lifetime management of the API resources.
    example_api(example_api &) = delete;
    auto operator=(example_api const &) -> example_api & = delete;
    example_api(example_api &&) = default;
    auto operator=(example_api &&) -> example_api & = default;

    // TODO: Expose public API here, and delegate to the PIMPL class internally
    void do_work(std::string const &message);

  private:
    // The real constructor will validate the version details provided by the caller matches
    // the version it was built with.
    //
    // This allows the detection of a mismatch between an installed library and the headers
    // that a client application is using.
    //
    // This could be anything unique and related to the release version, but we choose to use the
    // version number and the commit hash, as an example.
    struct expected_version
    {
      std::string_view version{ version::library_version };
      std::string_view commit{ version::git_hash };
      friend auto operator<=>(expected_version const &, expected_version const &) = default;
    };

    // private constructor that validates the version and does the real construction
    explicit example_api(expected_version callerVersion);

    std::unique_ptr<example_api_impl> m_impl;  // PIMPL
  };
}  // namespace safe_init::lib::example