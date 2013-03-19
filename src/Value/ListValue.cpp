#include "ListValue.h"

#include <ostream>

using namespace std;

namespace hap {

ListValue::ListValue(const ListValue& other) {
  for (const auto& value : other.values)
    values.push_back(unique_ptr<Value>(value->copy()));
}

bool ListValue::less(const Value& other) const {
  return Value::less(other)
    || (other.type() == Value::LIST
      && values < static_cast<const ListValue&>(other).values);
}

void ListValue::write(ostream& stream) const {
  stream << "[ ";
  for (const auto& value : values) {
    value->write(stream);
    stream << ", ";
  }
  stream << ']';
}

}
