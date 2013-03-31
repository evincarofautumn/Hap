#include "FloatValue.h"

#include <iomanip>
#include <ostream>

using namespace std;

namespace hap {

bool FloatValue::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const FloatValue*>(&expression)) {
    return value == that->value;
  }
  return false;
}

shared_ptr<Value> FloatValue::eval
  (Context&, const shared_ptr<Environment>) const {
  return shared_ptr<Value>(new FloatValue(*this));
}

bool FloatValue::less(const Value& that) const {
  return Value::less(that)
    || (that.type() == Value::FLOAT
      && value < static_cast<const FloatValue&>(that).value);
}

void FloatValue::write(ostream& stream) const {
  stream << setprecision(10) << showpoint << value;
}

}
