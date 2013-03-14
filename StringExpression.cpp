#include "StringExpression.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> StringExpression::eval(Environment&) const {
  return unique_ptr<Value>(new StringExpression(*this));
}

void StringExpression::write(ostream& stream) const {
  stream << '"' << value << '"';
}

}
