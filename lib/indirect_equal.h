#ifndef HAP_INDIRECT_EQUAL_H
#define HAP_INDIRECT_EQUAL_H

#include <functional>
#include <memory>

template<class T, class Equal = std::equal_to<T>>
class indirect_equal : public std::binary_function<T, T, bool> {
public:
  bool operator()(const T* const a, const T* const b) const {
    return equal(*a, *b);
  }
  bool operator()
    (const std::shared_ptr<T>& a,
     const std::shared_ptr<T>& b) const {
    return equal(*a, *b);
  }
  bool operator()
    (const std::shared_ptr<const T>& a,
     const std::shared_ptr<const T>& b) const {
    return equal(*a, *b);
  }
private:
  Equal equal;
};


template<class T, class U, class Equal = std::equal_to<T>>
class pair_indirect_equal
  : public std::binary_function<T, T, bool> {
private:
  typedef std::shared_ptr<const T> shared_T;
  typedef std::shared_ptr<const U> shared_U;
public:
  bool operator()
    (const std::pair<const T*, const U*>& a,
     const std::pair<const T*, const U*>& b) const {
    return equal(*a.first, *b.first) && equal(*a.second, *b.second);
  }
  bool operator()
    (const std::pair<shared_T, shared_U>& a,
     const std::pair<shared_T, shared_U>& b) const {
    return equal(*a.first, *b.first) && equal(*a.second, *b.second);
  }
private:
  Equal equal;
};

#endif
