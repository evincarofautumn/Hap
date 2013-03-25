#include "MapValue.h"

#include "indirect_equal.h"

#include <ostream>

using namespace std;

namespace hap {

bool MapValue::equal(const Expression& expression) const {
  if (auto other
      = dynamic_cast<const MapValue*>(&expression)) {
    return values.size() == other->values.size()
      && std::equal
        (begin(values), end(values),
         begin(other->values), pair_indirect_equal<Value, Value>());
  }
  return false;
}

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
