#ifndef HAP_MAPVALUE_H
#define HAP_MAPVALUE_H

#include "Value.h"
#include "indirect_compare.h"

#include <map>

namespace hap {

class MapValue : public Value {
public:
  MapValue() {}
  const std::shared_ptr<Value>& operator[](std::shared_ptr<Value>) const;
  std::shared_ptr<Value>& operator[](std::shared_ptr<Value>);
  void insert
    (const std::shared_ptr<Value> key,
     const std::shared_ptr<Value> value) {
    values.insert(std::make_pair(key, value));
  }
  virtual Value::Type type() const final override {
    return MAP;
  }
  virtual MapValue* copy() const final override {
    return new MapValue(*this);
  }
  virtual bool less(const Value&) const final override;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  MapValue(const MapValue&);
  std::map
    <std::shared_ptr<Value>,
     std::shared_ptr<Value>,
     indirect_compare<Value>> values;
};

template<>
struct value_traits<Value::MAP> {
  typedef MapValue type;
};

}

#endif
