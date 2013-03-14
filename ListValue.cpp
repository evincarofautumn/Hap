#include "ListValue.h"

#include <ostream>

using namespace std;

namespace hap {

ListValue::ListValue(const ListValue& other) {
  for (const auto& value : other.values)
    values.push_back(unique_ptr<Value>(value->copy()));
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
