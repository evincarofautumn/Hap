#include "StringValue.h"

#include <ostream>

using namespace std;

namespace hap {

bool StringValue::equal(const Expression& expression) const {
  if (auto other
      = dynamic_cast<const StringValue*>(&expression)) {
    return value == other->value;
  }
  return false;
}

shared_ptr<Value> StringValue::eval
  (Context&, const shared_ptr<Environment>) const {
  return shared_ptr<Value>(new StringValue(*this));
}

bool StringValue::less(const Value& other) const {
  return Value::less(other)
    || (other.type() == Value::STRING
      && value < static_cast<const StringValue&>(other).value);
}

void StringValue::write(ostream& stream) const {
  stream << '"' << value << '"';
}

}
