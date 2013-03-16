#ifndef HAP_MAPVALUE_H
#define HAP_MAPVALUE_H

#include "Value.h"

#include <map>

namespace hap {

class MapValue : public Value {
public:
  MapValue() {}
  void insert(std::unique_ptr<Value> key, std::unique_ptr<Value> value) {
    pairs.insert(std::make_pair(std::move(key), std::move(value)));
  }
  virtual Value::Type type() const final override {
    return Type::MAP;
  }
  virtual MapValue* copy() const final override {
    return new MapValue(*this);
  }
  virtual void write(std::ostream&) const final override;
private:
  MapValue(const MapValue&);
  std::map<std::unique_ptr<Value>, std::unique_ptr<Value>> pairs;
};

}

#endif
