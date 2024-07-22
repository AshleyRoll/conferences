#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <utility>

namespace safe_init::lib::example {

  // A simple thread safe queue.
  // This is no move/no copy because we hand out
  // producers and consumers which reference this class.
  class queue
  {
  public:
    static constexpr auto MaxLength = 50;

    // disable move and copy as we issue consumer/producer that saves a pointer to this class and accesses
    // its members, so we don't want to have copies or address changes.
    queue() = default;
    ~queue() = default;
    queue(queue &) = delete;
    auto operator=(queue const &) -> queue & = delete;
    queue(queue &&) = delete;
    auto operator=(queue &&) -> queue & = delete;

    class producer
    {
    public:
      explicit producer(queue *parent)
        : m_parent{ parent }
      {}

      void send(std::string const &message)
      {
        std::unique_lock lock(m_parent->m_lock);

        m_parent->m_notFull.wait(lock, [this] { return m_parent->m_queue.size() <= MaxLength; });

        m_parent->m_queue.push(message);
        m_parent->m_notEmpty.notify_one();
      }

    private:
      queue *m_parent;
    };

    class consumer
    {
    public:
      explicit consumer(queue *parent)
        : m_parent{ parent }
      {}

      [[nodiscard]] auto receive(std::chrono::milliseconds timeout) -> std::optional<std::string>
      {
        std::unique_lock lock(m_parent->m_lock);

        if (not m_parent->m_notEmpty.wait_for(lock, timeout, [this] { return not m_parent->m_queue.empty(); })) {
          return {};
        }

        auto message{ std::move(m_parent->m_queue.front()) };

        m_parent->m_queue.pop();
        m_parent->m_notFull.notify_one();

        return message;
      }

      [[nodiscard]] auto empty() -> bool
      {
        std::lock_guard const lock(m_parent->m_lock);
        return m_parent->m_queue.empty();
      }

    private:
      queue *m_parent;
    };

    [[nodiscard]] auto get_producer() -> producer { return producer{ this }; }
    [[nodiscard]] auto get_consumer() -> consumer { return consumer{ this }; }

  private:
    std::mutex m_lock;
    std::queue<std::string> m_queue;
    std::condition_variable m_notFull;
    std::condition_variable m_notEmpty;
  };

}  // namespace safe_init::lib::example