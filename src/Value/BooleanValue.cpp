#include "BooleanValue.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> BooleanValue::eval(Environment&) const {
  return unique_ptr<Value>(new BooleanValue(*this));
}

bool BooleanValue::less(const Value& other) const {
  return Value::less(other)
    || (other.type() == Value::BOOLEAN
      && value < static_cast<const BooleanValue&>(other).value);
}

void BooleanValue::write(ostream& stream) const {
  stream << (value ? "true" : "false");
}

}
