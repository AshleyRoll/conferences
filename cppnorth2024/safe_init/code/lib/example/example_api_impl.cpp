#include <string>

#include <fmt/core.h>

#include "example_api_impl.hpp"
#include "logger.hpp"

namespace safe_init::lib::example {
  example_api_impl::example_api_impl()
    : m_log{ "static_log" }
    , m_worker{ m_workQueue.get_consumer() }
  {
    fmt::println("example_api_impl::example_api_impl()");
  }

  example_api_impl::~example_api_impl() { fmt::println("example_api_impl::~example_api_impl()"); }

  void example_api_impl::do_work(std::string const &message)
  {
    log::info(fmt::format(R"(example_api_impl::do_work("{}"))", message));
    m_workQueue.get_producer().send(message);
  }
}  // namespace safe_init::lib::example
