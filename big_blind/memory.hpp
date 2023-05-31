#ifndef BIGBLINDCXX_MEMORY
#define BIGBLINDCXX_MEMORY

#include <tuple>
#include <type_traits>

namespace bigblind {
template <typename T>
struct default_deleter {
  constexpr default_deleter() noexcept = default;

  template <typename U, typename = std::is_convertible<U*, T*>>
  default_deleter(const default_deleter<U>&) noexcept {}

  void operator()(T* p) const {
    static_assert(!std::is_void<T>::value,
                  "can't delete pointer to incomplete type");
    static_assert(sizeof(T) > 0, "can't delete pointer to incomplete type");
    delete p;
  }
};

// Base class for shared_ptr and weak_ptr
template <typename T>
class ptr_base {
 public:
  ptr_base(const ptr_base&) = delete;

  ptr_base& operator=(const ptr_base&) = delete;

 private:
};

template <typename T, typename D>
class unique_ptr_impl {
 private:
  template <typename U, typename E, typename = void>
  struct Ptr {
    using type = U*;
  };

  template <typename U, typename E>
  struct Ptr<U,
             E,
             std::void_t<typename std::remove_reference<E>::type::pointer>> {
    using type = typename std::remove_reference<E>::type::pointer;
  };

 public:
  using DeleterConstraint =
      typename std::enable_if<!std::is_pointer<D>::value &&
                              std::is_default_constructible<D>::value>;

  using pointer = typename Ptr<T, D>::type;

  static_assert(!std::is_rvalue_reference<D>::value,
                "unique_ptr's deleter type must be function object type or "
                "an lvalue reference type");

  unique_ptr_impl() = default;
  unique_ptr_impl(pointer p) : m_t() { ptr() = p; }

  template <typename Del>
  unique_ptr_impl(pointer p, Del&& d) : m_t(p, std::forward<Del>(d)) {}

  unique_ptr_impl(unique_ptr_impl&& u) noexcept : m_t(std::move(u.m_t)) {
    u.ptr() = nullptr;
  }

  // unique_ptr_impl& operator=(unique_ptr_impl&& u) noexcept {
  //   reset(__u.release());
  //   _M_deleter() = std::forward<_Dp>(__u._M_deleter());
  //   return *this;
  // }

  pointer& ptr() { return std::get<0>(m_t); }
  pointer ptr() const { return std::get<0>(m_t); }
  D& deleter() { return std::get<1>(m_t); }
  const D& deleter() const { return std::get<1>(m_t); }

  // TODO...

 private:
  std::tuple<pointer, D> m_t;
};

struct zero_then_variadic_args_t {
  explicit zero_then_variadic_args_t() = default;
};

template <typename T,
          typename D,
          bool = std::is_move_constructible<D>::value,
          bool = std::is_move_assignable<D>::value>
struct unique_ptr_data : unique_ptr_impl<T, D> {
  using unique_ptr_impl<T, D>::unique_ptr_impl;
  unique_ptr_data(zero_then_variadic_args_t) {}
  unique_ptr_data(unique_ptr_data&&) = default;
  unique_ptr_data& operator=(unique_ptr_data&&) = default;
};

template <typename T, typename D>
struct unique_ptr_data<T, D, true, false> : unique_ptr_impl<T, D> {
  using unique_ptr_impl<T, D>::unique_ptr_impl;
  unique_ptr_data(unique_ptr_data&&) = default;
  unique_ptr_data& operator=(unique_ptr_data&&) = delete;
};

template <typename T, typename D>
struct unique_ptr_data<T, D, false, true> : unique_ptr_impl<T, D> {
  using unique_ptr_impl<T, D>::unique_ptr_impl;
  unique_ptr_data(unique_ptr_data&&) = delete;
  unique_ptr_data& operator=(unique_ptr_data&&) = default;
};

template <typename T, typename D>
struct unique_ptr_data<T, D, false, false> : unique_ptr_impl<T, D> {
  using unique_ptr_impl<T, D>::unique_ptr_impl;
  unique_ptr_data(unique_ptr_data&&) = delete;
  unique_ptr_data& operator=(unique_ptr_data&&) = delete;
};

template <typename T, typename Deleter = default_deleter<T>>
class unique_ptr {
 private:
  template <typename U>
  using DeleterConstraint =
      typename unique_ptr_impl<T, U>::DeleterConstraint::type;

 public:
  using pointer = typename unique_ptr_impl<T, Deleter>::pointer;
  using element_type = T;
  using deleter_type = Deleter;

  template <typename Del = Deleter, typename = DeleterConstraint<Del>>
  constexpr unique_ptr() noexcept : data(zero_then_variadic_args_t{}) {}

  template <typename Del = Deleter, typename = DeleterConstraint<Del>>
  explicit unique_ptr(pointer p) noexcept : data(p) {}

  // TODO...

  typename std::add_lvalue_reference<element_type>::type operator*()
      const noexcept {
    return *get();
  }

  pointer operator->() const noexcept { return get(); }

  pointer get() const noexcept { return data.ptr(); }

 private:
  unique_ptr_data<T, Deleter> data;
};

// template <typename T>
// class shared_ptr : public ptr_base {
//  public:
//  private:
// };

// template <typename T>
// class weak_ptr : public ptr_base {
//  public:
//  private:
// };
}  // namespace bigblind

#endif  // BIGBLINDCXX_MEMORY