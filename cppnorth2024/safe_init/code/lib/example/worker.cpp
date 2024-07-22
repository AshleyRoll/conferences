#include <chrono>
#include <stop_token>
#include <thread>

#include <fmt/core.h>

#include "logger.hpp"
#include "queue.hpp"
#include "worker.hpp"

namespace {

  using namespace safe_init::lib::example;

  inline constexpr std::chrono::milliseconds QueueTimeout{ 100 };

  void worker_process(std::stop_token const &stopToken, queue::consumer queueConsumer)
  {
    log::info("[worker] Starting...");

    while (!stopToken.stop_requested() || !queueConsumer.empty()) {
      auto message = queueConsumer.receive(QueueTimeout);
      if (message) { log::info(fmt::format("[worker] Received: {}", message.value())); }
    }

    log::info("[worker] Exiting");
  }

}  // namespace

namespace safe_init::lib::example {

  worker::worker(queue::consumer queueConsumer)
    : m_thread{ worker_process, queueConsumer }  // launches the worker thread
  {}

  worker::~worker()
  {
    // Asks the worker thread to stop and exit when it can.
    //
    // The destructor for the jthread will join() and so
    // will block until the work is complete.
    //
    // This means our destructor will block until the thread is complete as well.
    m_thread.request_stop();
  }

}  // namespace safe_init::lib::example