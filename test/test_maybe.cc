#include <cassert>
#include <string>

#include "../maybe.hh"

const char *const non_equal = "foo";

static void assert_empty(const maybe<std::string> &m) {
  assert(!m);
  assert(!m.get());
  assert(m.get_value_or("foo") == "foo");
}

static void assert_equal(const maybe<std::string> &m,
                         const std::string &equal) {
  assert(m);
  assert(m.get() == &*m);
  assert(m->size() == equal.size());
  assert(*m == equal);
  assert(*m != non_equal);
  assert(m.get_value_or(non_equal) == equal);
}

static void test_iterator() {
  maybe<int> a;
  assert(a.begin() == a.end());
  for (int i : a) { (void)i; assert(false); }

  a = 5;

  assert(a.begin() != a.end());
  assert(*a.begin() == 5);
  for (int i : a) {
    assert(i == 5);
  }
}

static void test_string() {
  maybe<std::string> a;
  assert_empty(a);
  a.clear();
  assert_empty(a);

  a = "bar";
  assert_equal(a, "bar");

  a.emplace(6, 'a');
  assert_equal(a, "aaaaaa");

  maybe<std::string> b("baz");
  assert_equal(b, "baz");

  a = b;
  assert_equal(a, "baz");
  a.clear();
  assert_empty(a);

  a = b;
  assert_equal(a, "baz");
}

int main() {
  test_iterator();
  test_string();
}
