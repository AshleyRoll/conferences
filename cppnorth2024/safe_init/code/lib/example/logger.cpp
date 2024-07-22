#include <string>
#include <utility>

#include <fmt/core.h>

#include "logger.hpp"

namespace safe_init::lib::example {

  logger::logger(std::string name)
    : m_name{ std::move(name) }
  {
    fmt::println("log::log()");
  }

  logger::~logger() { fmt::println("log::~log()"); }

  void logger::error(std::string const &message) { fmt::println("[ERR]({}) {}", m_name, message); }

  void logger::warn(std::string const &message) { fmt::println("[WRN]({}) {}", m_name, message); }

  void logger::info(std::string const &message) { fmt::println("[INF]({}) {}", m_name, message); }

}  // namespace safe_init::lib::example
