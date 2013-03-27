#include "MapValue.h"

#include "UndefinedValue.h"
#include "indirect_equal.h"

#include <ostream>

using namespace std;

namespace hap {

std::shared_ptr<Value>& MapValue::operator[]
  (const std::shared_ptr<Value> key) {
  const auto existing = find_if(begin(values), end(values),
    [key](const pair<const shared_ptr<Value>, shared_ptr<Value>>& entry) {
    return entry.first->equal(*key);
  });
  if (existing == end(values)) {
    values[key].reset(new UndefinedValue());
    return values[key];
  }
  return existing->second;
}

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
