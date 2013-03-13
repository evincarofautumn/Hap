#ifndef HAP_FLOW_H
#define HAP_FLOW_H

#include <memory>

namespace hap {

class Value;

namespace flow {

struct Last {};

struct Next {};

struct Return {
  Return(std::unique_ptr<Value> value)
    : value(std::move(value)) {}
  std::unique_ptr<Value> value;
};

}

}

#endif
