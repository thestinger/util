#include <cassert>

#include "../either.hh"

static void test_equality() {
  either<int, double> a(5.0), b(6.0);
  assert(a != b && !(a == b));
  b.emplace_right(5.0);
  assert(a == b && !(a != b));
  b.emplace_left(5);
  assert(a != b && !(a == b));
  a.emplace_left(5);
  assert(a == b && !(a != b));
  a.emplace_left(10);
  assert(a != b && !(a == b));
}

int main() {
  test_equality();
}
