#include "BooleanExpression.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> BooleanExpression::eval(Environment&) const {
  return unique_ptr<Value>(new BooleanExpression(*this));
}

void BooleanExpression::write(ostream& stream) const {
  stream << (value ? "true" : "false");
}

}
