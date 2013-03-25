#include "MapValue.h"

#include "indirect_equal.h"

#include <ostream>

using namespace std;

namespace hap {

bool MapValue::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const MapValue*>(&expression)) {
    return values.size() == that->values.size()
      && std::equal
        (begin(values), end(values),
         begin(that->values), pair_indirect_equal<Value, Value>());
  }
  return false;
}

MapValue::MapValue(const MapValue& that) {
  for (const auto& value : that.values)
    values.insert(make_pair
      (shared_ptr<Value>(value.first->copy()),
       shared_ptr<Value>(value.second->copy())));
}

bool MapValue::less(const Value& that) const {
  return Value::less(that)
    || (that.type() == Value::MAP
      && values < static_cast<const MapValue&>(that).values);
}

void MapValue::write(ostream& stream) const {
  stream << "{ ";
  for (const auto& value : values)
    stream << *value.first << ": " << *value.second << ", ";
  stream << '}';
}

}
