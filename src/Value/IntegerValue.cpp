#include "IntegerValue.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> IntegerValue::eval(Environment&) const {
  return unique_ptr<Value>(new IntegerValue(*this));
}

void IntegerValue::write(ostream& stream) const {
  stream << value;
}

}
