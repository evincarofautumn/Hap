#ifndef HAP_BOOLEANEXPRESSION_H
#define HAP_BOOLEANEXPRESSION_H

#include "Value.h"

namespace hap {

class BooleanExpression : public Value {
public:
  BooleanExpression(bool value)
    : value(value) {}
  virtual Value::Type type() const final override {
    return Type::BOOLEAN;
  }
  virtual BooleanExpression* copy() const final override {
    return new BooleanExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
  bool value;
};

}

#endif
