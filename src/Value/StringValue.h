#ifndef HAP_STRINGVALUE_H
#define HAP_STRINGVALUE_H

#include "Value.h"

#include <string>

namespace hap {

class StringValue : public Value {
public:
  StringValue(const std::string& value)
    : value(value) {}
  virtual Value::Type type() const final override {
    return Type::STRING;
  }
  virtual StringValue* copy() const final override {
    return new StringValue(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const final override;
  virtual bool less(const Value&) const final override;
  virtual void write(std::ostream&) const final override;
  std::string value;
};

}

#endif
