#include "StringValue.h"

#include <ostream>

using namespace std;

namespace hap {

bool StringValue::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const StringValue*>(&expression)) {
    return value == that->value;
  }
  return false;
}

shared_ptr<Value> StringValue::eval
  (Context&, const shared_ptr<Environment>) const {
  return make_shared<StringValue>(*this);
}

bool StringValue::less(const Value& that) const {
  return Value::less(that)
    || (that.type() == Value::STRING
      && value < static_cast<const StringValue&>(that).value);
}

void StringValue::write(ostream& stream) const {
  stream << '"' << value << '"';
}

}
