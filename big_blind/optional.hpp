#ifndef BIGBLINDCXX_OPTIONAL
#define BIGBLINDCXX_OPTIONAL

namespace bigblind {
struct nullopt_t {};
constexpr nullopt_t nullopt;

template <typename T>
class optional {
 public:
  optional() noexcept : has_value(false) {}

  optional(nullopt_t) {
    if (has_value) {
      value.~T();
      has_value = false;
    }
  }

  optional(T&& value) {
    destory();
    new (&this->value) T(std::move(value));
    has_value = true;
  }

  optional& operator=(const T& value) {
    destory();
    has_value = true;
    return *this;
  }

  optional& operator=(T&& value) {
    destory();
    new (&this->value) T(std::move(value));
    has_value = true;
    return *this;
  }

  ~optional() { destory(); }

  optional& operator=(nullopt_t) noexcept {
    if (has_value) {
      value.~T();
      has_value = false;
    }
    return *this;
  }

  template <typename V>
  constexpr T value_or(V&& v) const& {
    if (has_value)
      return this->value;
    return static_cast<T>(std::forward<V>(v));
  }

  template <typename V>
  constexpr T value_or(V&& v) && {
    if (has_value)
      return std::move(this->value);
    return static_cast<T>(std::forward<V>(v));
  }

  explicit operator bool() const { return this->has_value; }

  T& operator*() {
    static_assert(this->has_value);
    return this->value;
  }

  T const& operator*() const {
    static_assert(this->has_value);
    return this->value;
  }

 private:
  bool has_value;
  union {
    T value;
  };
  void destory() {
    if (has_value) {
      value.~T();
      has_value = false;
    }
  }
};
}  // namespace bigblind

#endif  // BIGBLINDCXX_OPTIONAL