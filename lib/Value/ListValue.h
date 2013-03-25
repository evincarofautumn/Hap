#ifndef HAP_LISTVALUE_H
#define HAP_LISTVALUE_H

#include "Value.h"

#include <vector>

namespace hap {

class ListValue : public Value {
public:
  ListValue() {}
  void push(std::shared_ptr<Value> value) {
    values.push_back(value);
  }
  virtual Value::Type type() const final override {
    return Type::LIST;
  }
  virtual ListValue* copy() const final override {
    return new ListValue(*this);
  }
  virtual bool less(const Value&) const final override;
  std::vector<std::shared_ptr<Value>> values;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  ListValue(const ListValue&);
};

template<>
struct value_traits<Value::Type::LIST> {
  typedef ListValue type;
};

}

#endif
