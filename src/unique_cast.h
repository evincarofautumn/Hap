#ifndef HAP_UNIQUE_CAST_H
#define HAP_UNIQUE_CAST_H

#include <memory>

template<typename Target, typename Source>
std::shared_ptr<Target> static_pointer_cast(std::unique_ptr<Source> pointer) {
  return std::shared_ptr<Target>(static_cast<Target*>(pointer.release()));
}

#endif
