#include "StringValue.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> StringValue::eval(Environment&) const {
  return unique_ptr<Value>(new StringValue(*this));
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
