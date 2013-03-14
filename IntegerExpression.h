#ifndef HAP_INTEGEREXPRESSION_H
#define HAP_INTEGEREXPRESSION_H

#include "Expression.h"
#include "Value.h"

namespace hap {

class IntegerExpression : virtual public Expression, public Value {
public:
  IntegerExpression(int value)
    : value(value) {}
  virtual Value::Type type() const override {
    return Type::INTEGER;
  }
  virtual IntegerExpression* copy() const override {
    return new IntegerExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
  int value;
};

}

#endif
