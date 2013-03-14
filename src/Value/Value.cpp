#include "Value.h"

#include <sstream>

using namespace std;

namespace hap {

Value::~Value() {}

void Value::assert_type(Type expected) const {
  auto actual(type());
  if (actual == expected)
    return;
  ostringstream message;
  message << "expected " << expected << " but got " << actual;
  throw runtime_error(message.str());
}

ostream& operator<<(ostream& stream, const Value::Type& type) {
  switch (type) {
  case Value::UNDEFINED: return stream << "undefined";
  case Value::BOOLEAN: return stream << "boolean";
  case Value::FUNCTION: return stream << "function";
  case Value::INTEGER: return stream << "integer";
  case Value::LIST: return stream << "list";
  case Value::MAP: return stream << "map";
  case Value::STRING: return stream << "string";
  }
}

}
