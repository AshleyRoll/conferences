#pragma once

#include <stdexcept>
#include <string>

namespace safe_init::lib::example {

  class library_version_mismatch : public std::logic_error
  {
  public:
    explicit library_version_mismatch(std::string const &message)
      : std::logic_error(message)
    {}
  };

  class library_instance_already_exists : public std::logic_error
  {
  public:
    explicit library_instance_already_exists()
      : std::logic_error("An instance of the api is already active. Only a single instance at a time is permitted")
    {}
  };

}  // namespace safe_init::lib::example