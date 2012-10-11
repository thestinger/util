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
    new (&memory) T(*reinterpret_cast<const T *>(&other.memory));
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
        *reinterpret_cast<T *>(&memory) = *reinterpret_cast<const T *>(&other.memory);
      } else {
        is_init = false;
        new (&memory) T(*reinterpret_cast<const T *>(&other.memory));
        is_init = true;
      }
    } else {
      is_init = false;
    }
    return *this;
  }

  maybe &operator=(maybe &&other) {
    assert(this != &other);
    if (other.is_init) {
      other.is_init = false;
      if (is_init) {
        *reinterpret_cast<T *>(&memory) =
          std::move(*reinterpret_cast<const T *>(&other.memory));
      } else {
        is_init = false;
        new (&memory) T(std::move(*reinterpret_cast<const T *>(&other.memory)));
        is_init = true;
      }
    } else {
      is_init = false;
    }
    return *this;
  }

  ~maybe() {
    if (is_init) {
      this->operator*().~T();
    }
  }

  explicit operator bool() const {
    return is_init;
  }

  T &operator*() {
    assert(is_init);
    return *reinterpret_cast<T *>(&memory);
  }

  const T &operator*() const {
    assert(is_init);
    return *reinterpret_cast<const T *>(&memory);
  }

  T *operator->() {
    assert(is_init);
    return reinterpret_cast<T *>(&memory);
  }

  const T *operator->() const {
    assert(is_init);
    return reinterpret_cast<const T *>(&memory);
  }

  T *get() {
    return is_init ? reinterpret_cast<T *>(&memory) : nullptr;
  }

  const T *get() const {
    return is_init ? reinterpret_cast<const T *>(&memory) : nullptr;
  }

  T const &get_value_or(T const &v) const {
    return is_init ? *reinterpret_cast<const T *>(&memory) : v;
  }

private:
  typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type memory;
  bool is_init;
};

#endif
