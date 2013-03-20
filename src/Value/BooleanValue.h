#ifndef HAP_BOOLEANVALUE_H
#define HAP_BOOLEANVALUE_H

#include "Value.h"

namespace hap {

class BooleanValue : public Value {
public:
  BooleanValue(bool value)
    : value(value) {}
  virtual Value::Type type() const final override {
    return Type::BOOLEAN;
  }
  virtual BooleanValue* copy() const final override {
    return new BooleanValue(*this);
  }
  virtual std::shared_ptr<Value> eval(std::shared_ptr<Environment>) const final override;
  virtual bool less(const Value&) const final override;
  virtual void write(std::ostream&) const final override;
  bool value;
};

}

#endif
