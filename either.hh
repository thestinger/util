#ifndef EITHER_HH
#define EITHER_HH

#include <functional>
#include <new>
#include <type_traits>
#include <utility>

template<typename Left, typename Right>
class either {
  static_assert(std::is_nothrow_move_constructible<Left>::value,
                "Left's move constructor must be noexcept");
  static_assert(std::is_nothrow_move_constructible<Right>::value,
                "Right's move constructor must be noexcept");

  void destroy() {
    if (is_left) {
      left.~Left();
    } else {
      right.~Right();
    }
  }

  union {
    Left left;
    Right right;
  };

  bool is_left;

public:
  either(const Left &other) : left(other), is_left(true) {}
  either(const Right &other) : right(other), is_left(false) {}
  either(Left &&other) : left(std::move(other)), is_left(true) {}
  either(Right &&other) : right(std::move(other)), is_left(false) {}

  either(const either &other) : is_left(other.is_left) {
    if (is_left) {
      new(&left) Left(other.left);
    } else {
      new(&right) Right(other.right);
    }
  }

  either(either &&other) noexcept : is_left(other.is_left) {
    if (is_left) {
      new(&left) Left(std::move(other.left));
    } else {
      new(&right) Right(std::move(other.right));
    }
  }

  either &operator=(either other) {
    destroy();
    is_left = other.is_left;
    if (is_left) {
      new(&left) Left(std::move(other.left));
    } else {
      new(&right) Right(std::move(other.right));
    }
    return *this;
  }

  ~either() {
    destroy();
  }

  template<typename LeftF, typename RightF>
  auto match(LeftF leftf, RightF rightf) -> decltype(leftf(left)) {
    return is_left ? leftf(left) : rightf(right);
  }

  template<typename LeftF, typename RightF>
  auto match(LeftF leftf, RightF rightf) const -> decltype(leftf(left)) {
    return is_left ? leftf(left) : rightf(right);
  }

  template<typename ...Args>
  void emplace_left(Args &&...args) {
    static_assert(std::is_nothrow_constructible<Left, Args...>::value,
                  "constructor must be noexcept");
    destroy();
    new(&left) Left(std::forward<Args>(args)...);
    is_left = true;
  }

  template<typename ...Args>
  void emplace_right(Args &&...args) {
    static_assert(std::is_nothrow_constructible<Right, Args...>::value,
                  "constructor must be noexcept");
    destroy();
    new(&right) Right(std::forward<Args>(args)...);
    is_left = false;
  }

  bool operator==(const either &other) const {
    return is_left == other.is_left &&
           is_left ? left == other.left : right == other.right;
  }

  bool operator!=(const either &other) const {
    return is_left != other.is_left ||
           is_left ? left != other.left : right != other.right;
  }

  friend struct std::hash<either<Left, Right>>;
};

namespace std {
  template<typename Left, typename Right>
  struct hash<either<Left, Right>> {
    hash() : hash_left(), hash_right() {}

    size_t operator()(const either<Left, Right> &e) const {
      return e.is_left ? ~hash_left(e.left) : hash_right(e.right);
    }

  private:
    const hash<Left> hash_left;
    const hash<Right> hash_right;
  };
}

#endif
