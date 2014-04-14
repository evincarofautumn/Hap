#include "UndefinedValue.h"

#include <ostream>

using namespace std;

namespace hap {

bool UndefinedValue::equal(const Expression& expression) const {
  return dynamic_cast<const UndefinedValue*>(&expression);
}

shared_ptr<Value> UndefinedValue::eval
  (Context&, const shared_ptr<Environment>) const {
  return make_shared<UndefinedValue>(*this);
}

bool UndefinedValue::less(const Value& that) const {
  return Value::less(that);
}

void UndefinedValue::write(ostream& stream) const {
  stream << "undefined";
}

}
