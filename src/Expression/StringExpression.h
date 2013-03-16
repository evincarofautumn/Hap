#ifndef HAP_STRINGEXPRESSION_H
#define HAP_STRINGEXPRESSION_H

#include "Value.h"

#include <string>

namespace hap {

class StringExpression : public Value {
public:
  StringExpression(const std::string& value)
    : value(value) {}
  virtual Value::Type type() const final override {
    return Type::STRING;
  }
  virtual StringExpression* copy() const final override {
    return new StringExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
  std::string value;
};

}

#endif
