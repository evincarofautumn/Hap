#ifndef HAP_UNDEFINEDEXPRESSION_H
#define HAP_UNDEFINEDEXPRESSION_H

#include "Value.h"

namespace hap {

class UndefinedExpression : public Value {
public:
  UndefinedExpression() = default;
  virtual Value::Type type() const final override {
    return Type::UNDEFINED;
  }
  virtual UndefinedExpression* copy() const final override {
    return new UndefinedExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  UndefinedExpression(const UndefinedExpression&) = default;
};

}

#endif
