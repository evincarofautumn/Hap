#ifndef HAP_INDIRECT_COMPARE_H
#define HAP_INDIRECT_COMPARE_H

#include <functional>
#include <memory>

template<class T, class Less = std::less<T>>
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
  Less compare;
};

#endif
