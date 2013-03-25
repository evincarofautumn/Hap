#include "UndefinedValue.h"

#include <ostream>

using namespace std;

namespace hap {

bool UndefinedValue::equal(const Expression& expression) const {
  return dynamic_cast<const UndefinedValue*>(&expression);
}

shared_ptr<Value> UndefinedValue::eval
  (Context&, const shared_ptr<Environment>) const {
  return shared_ptr<Value>(new UndefinedValue(*this));
}

bool UndefinedValue::less(const Value& other) const {
  return Value::less(other);
}

void UndefinedValue::write(ostream& stream) const {
  stream << "undefined";
}

}
