#ifndef CLAMP_HH
#define CLAMP_HH

#include <functional>

template<typename T, typename Pred>
T const &clamp(T const &val, T const &lo, T const &hi, Pred p) {
  return p(val, lo) ? lo : p(hi, val) ? hi : val;
}

template<typename T>
T const &clamp(const T &val, T const &lo, T const &hi) {
  return clamp(val, lo, hi, std::less<T>());
}

#endif
