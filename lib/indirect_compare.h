#ifndef HAP_INDIRECT_COMPARE_H
#define HAP_INDIRECT_COMPARE_H

#include <functional>
#include <memory>

template<class T, class Compare = std::less<T>>
class indirect_compare : public std::binary_function<T, T, bool> {
public:
  bool operator()(const T* const a, const T* const b) const {
    return compare(*a, *b);
  }
  bool operator()
    (const std::shared_ptr<const T>& a,
     const std::shared_ptr<const T>& b) const {
    return compare(*a, *b);
  }
  bool operator()
    (const std::shared_ptr<T>& a,
     const std::shared_ptr<T>& b) const {
    return compare(*a, *b);
  }
private:
  Compare compare;
};

template<class T, class U, class Compare = std::less<T>>
class pair_indirect_compare
  : public std::binary_function<T, T, bool> {
private:
  typedef std::shared_ptr<const T> shared_T;
  typedef std::shared_ptr<const U> shared_U;
public:
  bool operator()
    (const std::pair<const T*, const U*>& a,
     const std::pair<const T*, const U*>& b) const {
    return compare(*a.first, *b.first) && compare(*a.second, *b.second);
  }
  bool operator()
    (const std::pair<shared_T, shared_U>& a,
     const std::pair<shared_T, shared_U>& b) const {
    return compare(*a.first, *b.first) && compare(*a.second, *b.second);
  }
private:
  Compare compare;
};

template<class T>
using indirect_equal = indirect_compare<T, std::equal_to<T>>;

template<class T, class U>
using pair_indirect_equal = pair_indirect_compare<T, T, std::equal_to<T>>;

#endif
