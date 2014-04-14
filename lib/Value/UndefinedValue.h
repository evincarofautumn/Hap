#ifndef HAP_UNDEFINEDVALUE_H
#define HAP_UNDEFINEDVALUE_H

#include "Value.h"

namespace hap {

class UndefinedValue : public Value {
public:
  UndefinedValue() = default;
  UndefinedValue(const UndefinedValue&) = default;
  virtual Value::Type type() const final override {
    return UNDEFINED;
  }
  virtual UndefinedValue* copy() const final override {
    return new UndefinedValue(*this);
  }
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual bool less(const Value&) const final override;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
};

template<>
struct value_traits<Value::UNDEFINED> {
  typedef UndefinedValue type;
};

}

#endif
