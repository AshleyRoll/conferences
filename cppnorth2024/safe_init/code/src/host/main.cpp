#include <exception>

#include <fmt/core.h>

#include "example/example_api.hpp"

auto main() -> int
{
  using namespace safe_init::lib::example;

  {
    fmt::println("Api Instance 1");
    example_api api1{};
    api1.do_work("task 1");
    api1.do_work("task 2");
  }

  {
    fmt::println("Api Instance 2");
    example_api api2{};
    api2.do_work("task 3");
    api2.do_work("task 4");
  }

  {
    fmt::println("Api Instance 3");
    example_api const api3{};

    try {
      fmt::println("Attempting to create Api Instance 4");
      example_api const api4{};
      fmt::println("ERROR: Able to create Api Instance 4");
    } catch (std::exception &ex) {
      fmt::println("SUCCESS: Exception trying to create instance 4 - {}", ex.what());
    }
  }

  return 0;
}
