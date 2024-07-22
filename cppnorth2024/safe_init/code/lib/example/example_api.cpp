#include <memory>
#include <string>

#include <fmt/core.h>

#include <example/example_api.hpp>
#include <example/exceptions.hpp>

#include "example_api_impl.hpp"

namespace safe_init::lib::example {

  example_api::example_api(example_api::expected_version callerVersion)
    : m_impl{ std::make_unique<example_api_impl>() }
  {
    static expected_version expectedVersion{};

    fmt::println(R"(example_api::example_api({{"{}", "{}"}}))", callerVersion.version, callerVersion.commit);

    // Verify that the caller's version expectation matches our own.
    if (callerVersion != expectedVersion) {
      // Mismatch indicates that the statically linked or dynamically loaded library version (this code)
      // does not match the version that the host application was compiled against.
      throw library_version_mismatch{ fmt::format(R"(Library expected {{"{}", "{}"}}, Caller provided {{"{}", "{}"}})",
        expectedVersion.version,
        expectedVersion.commit,
        callerVersion.version,
        callerVersion.commit) };
    }
  }

  example_api::~example_api() { fmt::println("example_api::~example_api()"); }

  void example_api::do_work(std::string const &message) { m_impl->do_work(message); }

}  // namespace safe_init::lib::example
