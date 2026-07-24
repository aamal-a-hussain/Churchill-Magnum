#ifndef INCLUDE_UTILITIES_EVENTMANAGER_HPP_
#define INCLUDE_UTILITIES_EVENTMANAGER_HPP_

#include "Corrade/Utility/Debug.h"
#include <functional>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

struct EmptyEventArgs {};

template <typename T> class EventManager;

template <typename T> class Subscription {
public:
  using Listener = std::function<void(const T &)>;

  Subscription(typename std::vector<Listener>::iterator it,
               std::weak_ptr<EventManager<T>> manager)
      : m_iterator(it), m_manager(std::move(manager)), m_active(true) {};

  Subscription(const Subscription &) = delete;
  Subscription &operator=(const Subscription &) = delete;

  ~Subscription() {
    if (m_active) {
      if (auto mgr = m_manager.lock()) {
        mgr->unsubscribe(m_iterator);
      }
    }
  }

private:
  typename std::vector<Listener>::iterator m_iterator;
  std::weak_ptr<EventManager<T>> m_manager;
  bool m_active;
};

template <typename T>
class EventManager : public std::enable_shared_from_this<EventManager<T>> {
public:
  using Listener = std::function<void(const T &)>;
  using SubscriptionPtr = std::shared_ptr<Subscription<T>>;

  static std::shared_ptr<EventManager<T>> create() {
    return std::shared_ptr<EventManager<T>>(new EventManager<T>);
  };

  [[nodiscard]] SubscriptionPtr subscribe(Listener listener) {
    m_listeners.push_back(std::move(listener));
    auto it = std::prev(m_listeners.end());
    return std::make_shared<Subscription<T>>(it, this->weak_from_this());
  }

  void unsubscribe(typename std::vector<Listener>::iterator it) {
    m_listeners.erase(it);
  }

  void publish(const T &args) {
    for (const auto &l : m_listeners)
      if (l)
        l(args);
  }

private:
  EventManager() = default;
  std::vector<Listener> m_listeners;
};

#endif // INCLUDE_UTILITIES_EVENTMANAGER_HPP_
