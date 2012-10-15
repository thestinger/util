#ifndef MAYBE_HH
#define MAYBE_HH

#include <cassert>
#include <new>
#include <memory>
#include <type_traits>

template<typename T>
struct maybe {
  maybe() noexcept : is_init(false) {}

  maybe(const T &other) noexcept(std::is_nothrow_copy_constructible<T>::value) : is_init(false) {
    new (&memory) T(other);
    is_init = true;
  }

  maybe(T &&other) noexcept(std::is_nothrow_move_constructible<T>::value) : is_init(false) {
    new (&memory) T(std::move(other));
    is_init = true;
  }

  maybe(const maybe &other) noexcept(std::is_nothrow_copy_constructible<T>::value) : is_init(false) {
    if (other.is_init) {
      new (&memory) T(*other.as_ptr());
      is_init = other.is_init;
    }
  }

  maybe(maybe &&other) noexcept(std::is_nothrow_move_constructible<T>::value) : is_init(false) {
    if (other.is_init) {
      new (&memory) T(std::move(*other.as_ptr()));
      other.is_init = false;
      is_init = true;
    }
  }

  maybe &operator=(const maybe &other) {
    if (this != &other && other.is_init) {
      if (is_init) {
        *as_ptr() = *other.as_ptr();
      } else {
        new (&memory) T(*other.as_ptr());
        is_init = true;
      }
    } else {
      clear();
    }
    return *this;
  }

  maybe &operator=(maybe &&other) {
    assert(this != &other);
    if (other.is_init) {
      other.is_init = false;
      if (is_init) {
        *as_ptr() = std::move(*other.as_ptr());
      } else {
        new (&memory) T(std::move(*other.as_ptr()));
        is_init = true;
      }
    } else {
      clear();
    }
    return *this;
  }

  ~maybe() {
    if (is_init) {
      as_ptr()->~T();
    }
  }

  explicit operator bool() const noexcept {
    return is_init;
  }

  T &operator*() noexcept {
    assert(is_init);
    return *as_ptr();
  }

  const T &operator*() const noexcept {
    assert(is_init);
    return *as_ptr();
  }

  T *operator->() noexcept {
    assert(is_init);
    return as_ptr();
  }

  const T *operator->() const noexcept {
    assert(is_init);
    return as_ptr();
  }

  T *get() noexcept {
    return is_init ? as_ptr() : nullptr;
  }

  const T *get() const noexcept {
    return is_init ? as_ptr() : nullptr;
  }

  T const &get_value_or(T const &v) const noexcept {
    return is_init ? *as_ptr() : v;
  }

  void clear() noexcept(std::is_nothrow_destructible<T>::value) {
    if (is_init) {
      as_ptr()->~T();
      is_init = false;
    }
  }

private:
  T *as_ptr() { return reinterpret_cast<T *>(&memory); }
  const T *as_ptr() const { return reinterpret_cast<const T *>(&memory); }

  typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type memory;
  bool is_init;
};

#endif
