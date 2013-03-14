#ifndef HAP_UNDEFINEDEXPRESSION_H
#define HAP_UNDEFINEDEXPRESSION_H

#include "Expression.h"
#include "Value.h"

namespace hap {

class UndefinedExpression : public Value {
public:
  UndefinedExpression() = default;
  virtual Value::Type type() const override {
    return Type::UNDEFINED;
  }
  virtual UndefinedExpression* copy() const override {
    return new UndefinedExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  UndefinedExpression(const UndefinedExpression&) = default;
};

}

#endif
