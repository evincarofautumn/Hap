#ifndef HAP_INTEGERVALUE_H
#define HAP_INTEGERVALUE_H

#include "Value.h"

namespace hap {

class IntegerValue : public Value {
public:
  IntegerValue(int value)
    : value(value) {}
  virtual Value::Type type() const final override {
    return Type::INTEGER;
  }
  virtual IntegerValue* copy() const final override {
    return new IntegerValue(*this);
  }
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual bool less(const Value&) const final override;
  virtual void write(std::ostream&) const final override;
  int value;
};

template<>
struct value_traits<Value::Type::INTEGER> {
  typedef IntegerValue type;
};

}

#endif
