#include "ListValue.h"

#include "indirect_equal.h"

#include <ostream>

using namespace std;

namespace hap {

bool ListValue::equal(const Expression& expression) const {
  if (auto other
      = dynamic_cast<const ListValue*>(&expression)) {
    return values.size() == other->values.size()
      && std::equal
        (begin(values), end(values),
         begin(other->values), indirect_equal<Value>());
  }
  return false;
}

ListValue::ListValue(const ListValue& other) {
  for (const auto& value : other.values)
    values.push_back(shared_ptr<Value>(value->copy()));
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
