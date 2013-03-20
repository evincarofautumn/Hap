#include "IntegerValue.h"

#include <ostream>

using namespace std;

namespace hap {

shared_ptr<Value> IntegerValue::eval
  (const shared_ptr<Environment>) const {
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
