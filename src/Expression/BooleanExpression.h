#ifndef HAP_BOOLEANEXPRESSION_H
#define HAP_BOOLEANEXPRESSION_H

#include "Expression.h"
#include "Value.h"

namespace hap {

class BooleanExpression : public Value {
public:
  BooleanExpression(bool value)
    : value(value) {}
  virtual Value::Type type() const override {
    return Type::BOOLEAN;
  }
  virtual BooleanExpression* copy() const override {
    return new BooleanExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
  bool value;
};

}

#endif
