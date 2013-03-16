#ifndef HAP_INTEGEREXPRESSION_H
#define HAP_INTEGEREXPRESSION_H

#include "Value.h"

namespace hap {

class IntegerExpression : public Value {
public:
  IntegerExpression(int value)
    : value(value) {}
  virtual Value::Type type() const final override {
    return Type::INTEGER;
  }
  virtual IntegerExpression* copy() const final override {
    return new IntegerExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
  int value;
};

}

#endif
