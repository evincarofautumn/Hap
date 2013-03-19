#include "MapValue.h"

#include <ostream>

using namespace std;

namespace hap {

MapValue::MapValue(const MapValue& other) {
  for (const auto& pair : other.pairs)
    pairs.insert (make_pair
      (unique_ptr<Value>(pair.first->copy()),
       unique_ptr<Value>(pair.second->copy())));
}

bool MapValue::less(const Value& other) const {
  return Value::less(other)
    || (other.type() == Value::MAP
      && pairs < static_cast<const MapValue&>(other).pairs);
}

void MapValue::write(ostream& stream) const {
  stream << "{ ";
  for (const auto& pair : pairs) {
    pair.first->write(stream);
    stream << ": ";
    pair.second->write(stream);
    stream << ", ";
  }
  stream << '}';
}

}
