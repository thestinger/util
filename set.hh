#ifndef SET_HH
#define SET_HH

#include <algorithm>
#include <set>

template<typename T>
std::set<T> operator|(std::set<T> a, std::set<T> b) {
  std::set<T> c;
  std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                 std::inserter(c, c.end()));
  return c;
}

template<typename T>
std::set<T> operator&(std::set<T> a, std::set<T> b) {
  std::set<T> c;
  std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                        std::inserter(c, c.end()));
  return c;
}

template<typename T>
std::set<T> operator-(std::set<T> a, std::set<T> b) {
  std::set<T> c;
  std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                      std::inserter(c, c.end()));
  return c;
}

template<typename T>
std::set<T> operator^(std::set<T> a, std::set<T> b) {
  std::set<T> c;
  std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                                std::inserter(c, c.end()));
  return c;
}

#endif
