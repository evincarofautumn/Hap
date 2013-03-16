#ifndef HAP_LISTVALUE_H
#define HAP_LISTVALUE_H

#include "Value.h"

#include <vector>

namespace hap {

class ListValue : public Value {
public:
  ListValue() {}
  void push(std::unique_ptr<Value> value) {
    values.push_back(std::move(value));
  }
  virtual Value::Type type() const final override {
    return Type::LIST;
  }
  virtual ListValue* copy() const final override {
    return new ListValue(*this);
  }
  virtual void write(std::ostream&) const final override;
private:
  ListValue(const ListValue&);
  std::vector<std::unique_ptr<Value>> values;
};

}

#endif
