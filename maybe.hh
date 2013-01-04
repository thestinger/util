#ifndef MAYBE_HH
#define MAYBE_HH

#include <cassert>
#include <initializer_list>
#include <new>
#include <type_traits>
#include <utility>

template<typename T>
struct maybe {
  constexpr maybe() noexcept : is_init(false) {}

  template<typename ...Args>
  maybe(Args &&...args) : is_init(false) {
    new(&memory) T(std::forward<Args>(args)...);
    is_init = true;
  }

  template<typename U,
           typename = typename std::enable_if<std::is_constructible<T, std::initializer_list<U>>::value>::type>
  maybe(std::initializer_list<U> init) : is_init(false) {
    new(&memory) T(std::forward<std::initializer_list<U>>(init));
    is_init = true;
  }

  maybe(const maybe &other) noexcept(std::is_nothrow_copy_constructible<T>::value) : is_init(false) {
    if (other.is_init) {
      new (&memory) T(*other);
      is_init = true;
    }
  }

  maybe(maybe &&other) noexcept(std::is_nothrow_move_constructible<T>::value) : is_init(false) {
    if (other.is_init) {
      new (&memory) T(std::move(*other));
      is_init = true;
    }
  }

  maybe &operator=(const maybe &other) {
    if (this != &other && other.is_init) {
      if (is_init) {
        memory = *other;
      } else {
        new (&memory) T(*other);
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
      if (is_init) {
        memory = std::move(*other);
      } else {
        new (&memory) T(std::move(*other));
        is_init = true;
      }
    } else {
      clear();
    }
    return *this;
  }

  ~maybe() {
    if (is_init) {
      memory.~T();
    }
  }

  explicit operator bool() const noexcept {
    return is_init;
  }

  T &operator*() noexcept {
    assert(is_init);
    return memory;
  }

  const T &operator*() const noexcept {
    assert(is_init);
    return memory;
  }

  T *operator->() noexcept {
    assert(is_init);
    return &memory;
  }

  const T *operator->() const noexcept {
    assert(is_init);
    return &memory;
  }

  T *get() noexcept {
    return is_init ? &memory : nullptr;
  }

  const T *get() const noexcept {
    return is_init ? &memory : nullptr;
  }

  T const &get_value_or(T const &v) const noexcept {
    return is_init ? memory : v;
  }

  void clear() {
    if (is_init) {
      memory.~T();
      is_init = false;
    }
  }

  template<typename ...Args>
  void emplace(Args &&...args) {
    clear();
    new(&memory) T(std::forward<Args>(args)...);
    is_init = true;
  }

private:
  union { T memory; };
  bool is_init;
};

#endif
