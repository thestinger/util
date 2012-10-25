#ifndef EITHER_HH
#define EITHER_HH

#include <new>
#include <type_traits>

template<typename Left, typename Right,
         typename = typename std::enable_if<std::is_nothrow_move_constructible<Left>::value &&
                                            std::is_nothrow_move_constructible<Right>::value
                                           >::type
        >
struct either {
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

  either operator=(either other) {
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

private:
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
  template<typename LeftF, typename RightF>
  auto match(LeftF leftf, RightF rightf) -> decltype(leftf(left)) {
    if (is_left) {
      return leftf(left);
    } else {
      return rightf(right);
    }
  }

  template<typename LeftF, typename RightF>
  auto match(LeftF leftf, RightF rightf) const -> decltype(leftf(left)) {
    if (is_left) {
      return leftf(left);
    } else {
      return rightf(right);
    }
  }
};

#endif
