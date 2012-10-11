#ifndef MAYBE_HH
#define MAYBE_HH

#include <cassert>
#include <cstring>
#include <new>
#include <memory>

template<typename T>
struct maybe {
  maybe() : is_init(false) {}

  maybe(const T &other) : is_init(false) {
    new (&memory) T(other);
    is_init = true;
  }

  maybe(const maybe &other) : is_init(false) {
    new (&memory) T(*other.as_ptr());
    is_init = other.is_init;
  }

  maybe(maybe &&other) noexcept : is_init(other.is_init) {
    memcpy(reinterpret_cast<char *>(&memory),
           reinterpret_cast<char *>(&other.memory), sizeof(T));
    other.is_init = false;
  }

  maybe &operator=(const maybe &other) {
    if (this != &other && other.is_init) {
      if (is_init) {
        *as_ptr() = *other.as_ptr();
      } else {
        new (&memory) T(*other.as_ptr());
        is_init = true;
      }
    } else if (is_init) {
      as_ptr()->~T();
      is_init = false;
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
    } else if (is_init) {
      as_ptr()->~T();
      is_init = false;
    }
    return *this;
  }

  ~maybe() {
    if (is_init) {
      as_ptr()->~T();
    }
  }

  explicit operator bool() const {
    return is_init;
  }

  T &operator*() {
    assert(is_init);
    return *as_ptr();
  }

  const T &operator*() const {
    assert(is_init);
    return *as_ptr();
  }

  T *operator->() {
    assert(is_init);
    return as_ptr();
  }

  const T *operator->() const {
    assert(is_init);
    return as_ptr();
  }

  T *get() {
    return is_init ? as_ptr() : nullptr;
  }

  const T *get() const {
    return is_init ? as_ptr() : nullptr;
  }

  T const &get_value_or(T const &v) const {
    return is_init ? *as_ptr() : v;
  }

private:
  T *as_ptr() { return reinterpret_cast<T *>(&memory); }
  const T *as_ptr() const { return reinterpret_cast<const T *>(&memory); }

  typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type memory;
  bool is_init;
};

#endif
