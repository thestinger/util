#ifndef EITHER_HH
#define EITHER_HH

template<typename Left, typename Right>
struct either {
  either(const Left &other) : left(other), is_left(true) {}
  either(const Right &other) : right(other), is_left(false) {}
  either(Left &&other) : left(std::move(other)), is_left(true) {}
  either(Right &&other) : right(std::move(other)), is_left(false) {}

  template<typename Visitor>
  void apply_visitor(Visitor visitor) {
    if (is_left) {
      visitor(left);
    } else {
      visitor(right);
    }
  }

  template<typename Visitor>
  void apply_visitor(Visitor visitor) const {
    if (is_left) {
      visitor(left);
    } else {
      visitor(right);
    }
  }

  ~either() {
    if (is_left) {
      left.~Left();
    } else {
      right.~Right();
    }
  }

private:
  union {
    Left left;
    Right right;
  };

  bool is_left;
};

#endif
