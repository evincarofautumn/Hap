#ifndef HAP_FLOATVALUE_H
#define HAP_FLOATVALUE_H

#include "Value.h"

#include <cstdint>

namespace hap {

class FloatValue : public Value {
public:
  FloatValue(double value)
    : value(value) {}
  virtual Value::Type type() const final override {
    return FLOAT;
  }
  virtual FloatValue* copy() const final override {
    return new FloatValue(*this);
  }
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual bool less(const Value&) const final override;
  double value;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
};

template<>
struct value_traits<Value::FLOAT> {
  typedef FloatValue type;
};

}

#endif
