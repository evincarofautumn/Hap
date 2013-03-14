#ifndef HAP_UNIQUE_CAST_H
#define HAP_UNIQUE_CAST_H

#include <memory>

template<typename Target, typename Source>
std::unique_ptr<Target> static_unique_cast(std::unique_ptr<Source> pointer) {
  return std::unique_ptr<Target>(static_cast<Target*>(pointer.release()));
}

#endif
