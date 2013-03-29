#ifndef HAP_FLOW_H
#define HAP_FLOW_H

#include "Value.h"

#include <memory>

namespace hap {

class Value;

namespace flow {

struct Exit {};

struct Last {};

struct Next {};

struct Return {
  Return(std::shared_ptr<Value> value)
    : value(value) {}
  std::shared_ptr<Value> value;
};

}

}

#endif
