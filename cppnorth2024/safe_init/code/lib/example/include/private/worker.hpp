#pragma once

#include <thread>

#include "queue.hpp"

namespace safe_init::lib::example {

  // Manages a thread lifetime and processes work from the
  // work queue.
  class worker
  {
  public:
    explicit worker(queue::consumer queueConsumer);
    ~worker();

  private:
    std::jthread m_thread;
  };

}  // namespace safe_init::lib::example