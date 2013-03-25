#include "IntegerValue.h"

#include <ostream>

using namespace std;

namespace hap {

bool IntegerValue::equal(const Expression& expression) const {
  if (auto other
      = dynamic_cast<const IntegerValue*>(&expression)) {
    return value == other->value;
  }
  return false;
}

shared_ptr<Value> IntegerValue::eval
  (Context&, const shared_ptr<Environment>) const {
  return shared_ptr<Value>(new IntegerValue(*this));
}

bool IntegerValue::less(const Value& other) const {
  return Value::less(other)
    || (other.type() == Value::INTEGER
      && value < static_cast<const IntegerValue&>(other).value);
}

void IntegerValue::write(ostream& stream) const {
  stream << value;
}

}
