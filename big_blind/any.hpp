#ifndef BIGBLINDCXX_ANY
#define BIGBLINDCXX_ANY

#include <type_traits>
#include <typeinfo>

namespace bigblind {
class any {
 public:
  any() : content(nullptr) {}

  any(const any& other) : content(other.content ? other.content->clone() : 0) {}

  any(any&& other) : content(other.content) { other.content = nullptr; }

  template <typename ValueType>
  any(const ValueType& value) : content(new holder<ValueType>(value)) {}

  template <typename ValueType>
  any(ValueType&& value)
      : content(new holder<ValueType>(static_cast<ValueType&&>(value))) {}

  ~any() { reset(); }

  any& operator=(const any& rhs) {
    any(rhs).swap(*this);
    return *this;
  }

  any& operator=(any&& rhs) {
    rhs.swap(*this);
    any().swap(rhs);
    return *this;
  }

  template <typename ValueType>
  any& operator=(const ValueType& rhs) {
    any(rhs).swap(*this);
    return *this;
  }

  template <typename ValueType>
  any& operator=(ValueType&& rhs) {
    any(static_cast<ValueType&&>(rhs)).swap(*this);
    return *this;
  }

  const std::type_info& type() const {
    if (!has_value())
      return typeid(void);
    return content->type();
  }

  bool has_value() const noexcept { return content != nullptr; }

  void reset() noexcept {
    if (content) {
      delete content;
      content = nullptr;
    }
  }

  any& swap(any& rhs) {
    placeholder* tmp = content;
    content = rhs.content;
    rhs.content = tmp;
    return *this;
  }

  class placeholder {
   public:
    virtual placeholder* clone() const = 0;
    virtual const std::type_info& type() const = 0;
  };

  template <typename ValueType>
  class holder : public placeholder {
   public:
    holder(const ValueType& value) : held(value) {}
    placeholder* clone() const override { return new holder<ValueType>(held); }
    const std::type_info& type() const { return typeid(ValueType); }

    ValueType held;
  };

 private:
  placeholder* content;
};
}  // namespace bigblind

#endif  // BIGBLINDCXX_ANY