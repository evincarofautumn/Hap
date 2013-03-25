#include "BinaryExpression.h"
#include "Value.h"

#include <sstream>

using namespace std;

namespace hap {

shared_ptr<Value> BinaryExpression::eval
  (Context& context,
   const shared_ptr<Environment> environment) const {
  if (operator_.binary)
    return operator_.binary(context, environment, left, right);
  ostringstream message;
  message << "unimplemented binary operator " << operator_;
  throw runtime_error(message.str());
}

bool BinaryExpression::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const BinaryExpression*>(&expression)) {
    return *left == *that->left
      && *right == *that->right;
  }
  return false;
}

void BinaryExpression::write(ostream& stream) const {
  stream << '(' << *left << ' ' << operator_ << ' ' << *right << ')';
}

}
