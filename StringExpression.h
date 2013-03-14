#ifndef HAP_STRINGEXPRESSION_H
#define HAP_STRINGEXPRESSION_H

#include "Expression.h"
#include "Value.h"

namespace hap {

class StringExpression : public Value {
public:
  StringExpression(const std::string& value)
    : value(value) {}
  virtual Value::Type type() const override {
    return Type::STRING;
  }
  virtual StringExpression* copy() const override {
    return new StringExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
  std::string value;
};

}

#endif
