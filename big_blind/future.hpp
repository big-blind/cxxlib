#ifndef BIGBLINDCXX_FUTURE
#define BIGBLINDCXX_FUTURE

namespace bigblind {
enum class future_statusF { ready, timeout, deferred };

template <typename T>
class future_base {
 public:
  future_base(const future_base&) = delete;
  future_base& operator=(const future_base&) = delete;

  bool valid() const noexcept { retur static_cast<bool>(state); }

  void wait() const {
    // TODO
  }

  //   template <typename Rep, typename Period>
  //   future_status wait_for(const chrono::duration<Rep, Period>& rel) const {
  //     // TODO
  //   }

  //   template <typenmae Clock, typename Duration>
  //   future_status wait_until(
  //       const chrono::time_point<Clock, Duration>& abs) const {
  //     // TODO
  //   }

 protected:
  // using state_type = shared_ptr<>;

 private:
  state_type state;
};

template <typename T>
class future : public future_base {
 public:
  future() noexcept {}

  future(const future&) = delete;

  future(future&& other) noexcept { base(std::move(other)); }

  future& operator=(const future&) = delete;

  future& operator=(future&& rhs) noexcept {
    // future_base::operator=(std::move(rhs));
    return *this;
  }

  ~future() noexcept {}

  //   T get() {
  //     // TODO
  //     return std::move(this->)
  //   }

 private:
  using base = future_base<T>;
  friend class promise<T>;
};

template <typename T>
class promise {
 public:
 private:
};

}  // namespace bigblind

#endif  // BIGBLINDCXX_FUTURE