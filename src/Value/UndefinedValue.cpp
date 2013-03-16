#include "UndefinedValue.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> UndefinedValue::eval(Environment& environment) const {
  return unique_ptr<Value>(new UndefinedValue(*this));
}

void UndefinedValue::write(ostream& stream) const {
  stream << "undefined";
}

}
