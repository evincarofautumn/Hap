#include "StringValue.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> StringValue::eval(Environment&) const {
  return unique_ptr<Value>(new StringValue(*this));
}

void StringValue::write(ostream& stream) const {
  stream << '"' << value << '"';
}

}
