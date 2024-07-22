#pragma once

#include <string>

#include "scoped_singleton.hpp"

namespace safe_init::lib::example {

  // A placeholder for a real logging system
  class logger  // NOLINT(cppcoreguidelines-special-member-functions)
  {
  public:
    explicit logger(std::string name);
    ~logger();

    [[maybe_unused]] void error(std::string const &message);
    [[maybe_unused]] void warn(std::string const &message);
    [[maybe_unused]] void info(std::string const &message);

  private:
    std::string m_name;
  };


  // Define helpers for the global singleton logger. The instance of this
  // is managed by the example_api_impl.
  namespace log {
    [[maybe_unused]] inline static void error(std::string const &message)
    {
      scoped_singleton<logger>::instance().error(message);
    }

    [[maybe_unused]] inline static void warn(std::string const &message)
    {
      scoped_singleton<logger>::instance().warn(message);
    }

    [[maybe_unused]] inline static void info(std::string const &message)
    {
      scoped_singleton<logger>::instance().info(message);
    }
  }  // namespace log


}  // namespace safe_init::lib::example