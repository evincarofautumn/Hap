#include "IntegerExpression.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> IntegerExpression::eval(Environment&) const {
  return unique_ptr<Value>(new IntegerExpression(*this));
}

void IntegerExpression::write(ostream& stream) const {
  stream << value;
}

}
