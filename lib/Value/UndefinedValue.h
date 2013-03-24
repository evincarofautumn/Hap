#ifndef HAP_UNDEFINEDVALUE_H
#define HAP_UNDEFINEDVALUE_H

#include "Value.h"

namespace hap {

class UndefinedValue : public Value {
public:
  UndefinedValue() = default;
  virtual Value::Type type() const final override {
    return Type::UNDEFINED;
  }
  virtual UndefinedValue* copy() const final override {
    return new UndefinedValue(*this);
  }
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual bool less(const Value&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  UndefinedValue(const UndefinedValue&) = default;
};

template<>
struct value_traits<Value::Type::UNDEFINED> {
  typedef UndefinedValue type;
};

}

#endif
