#include "BinaryExpression.h"
#include "Value.h"

#include <sstream>

using namespace std;

namespace hap {

unique_ptr<Value> BinaryExpression::eval(Environment& environment) const {
  if (operator_.binary)
    return operator_.binary(environment, left, right);
  ostringstream message;
  message << "unimplemented binary operator " << operator_;
  throw runtime_error(message.str());
}

void BinaryExpression::write(ostream& stream) const {
  stream << '(';
  left->write(stream);
  stream << ' ' << operator_ << ' ';
  right->write(stream);
  stream << ')';
}

}
