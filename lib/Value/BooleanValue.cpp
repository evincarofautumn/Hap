#include "BooleanValue.h"

#include <ostream>

using namespace std;

namespace hap {

bool BooleanValue::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const BooleanValue*>(&expression)) {
    return value == that->value;
  }
  return false;
}

shared_ptr<Value> BooleanValue::eval
  (Context&, const shared_ptr<Environment>) const {
  return shared_ptr<Value>(new BooleanValue(*this));
}

bool BooleanValue::less(const Value& that) const {
  return Value::less(that)
    || (that.type() == Value::BOOLEAN
      && value < static_cast<const BooleanValue&>(that).value);
}

void BooleanValue::write(ostream& stream) const {
  stream << (value ? "true" : "false");
}

}
