#include "IntegerValue.h"

#include <ostream>

using namespace std;

namespace hap {

bool IntegerValue::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const IntegerValue*>(&expression)) {
    return value == that->value;
  }
  return false;
}

shared_ptr<Value> IntegerValue::eval
  (Context&, const shared_ptr<Environment>) const {
  return shared_ptr<Value>(new IntegerValue(*this));
}

bool IntegerValue::less(const Value& that) const {
  return Value::less(that)
    || (that.type() == Value::INTEGER
      && value < static_cast<const IntegerValue&>(that).value);
}

void IntegerValue::write(ostream& stream) const {
  stream << value;
}

}
