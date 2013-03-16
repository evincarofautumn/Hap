#include "BooleanValue.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> BooleanValue::eval(Environment&) const {
  return unique_ptr<Value>(new BooleanValue(*this));
}

void BooleanValue::write(ostream& stream) const {
  stream << (value ? "true" : "false");
}

}
