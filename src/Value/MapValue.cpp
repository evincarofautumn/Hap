#include "MapValue.h"

#include <ostream>

using namespace std;

namespace hap {

MapValue::MapValue(const MapValue& other) {
  for (const auto& value : other.values)
    values.insert(make_pair
      (shared_ptr<Value>(value.first->copy()),
       shared_ptr<Value>(value.second->copy())));
}

bool MapValue::less(const Value& other) const {
  return Value::less(other)
    || (other.type() == Value::MAP
      && values < static_cast<const MapValue&>(other).values);
}

void MapValue::write(ostream& stream) const {
  stream << "{ ";
  for (const auto& value : values) {
    value.first->write(stream);
    stream << ": ";
    value.second->write(stream);
    stream << ", ";
  }
  stream << '}';
}

}
