#include <cassert>
#include <unordered_map>

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

static void test_hash() {
  std::unordered_map<either<int, double>, unsigned> map;
  map[5.5] = 10;
  map[5.5] += 10;
  map[5] = 15;
  assert(map[5.5] == 20);
  assert(map[5] == 15);
  assert(map[6] == 0);
  assert(map.find(5.0) == map.end());
}

static void test_match() {
  either<int, double> a(5);
  assert(a.match([](int) { return 1; }, [](double) { return 2; }) == 1);
  a.emplace_right(5.5);
  assert(a.match([](int) { return 1; }, [](double) { return 2; }) == 2);
}

int main() {
  test_equality();
  test_hash();
  test_match();
}
