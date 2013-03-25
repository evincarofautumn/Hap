#include "IdentifierExpression.h"

#include "Environment.h"
#include "Value.h"

using namespace std;

namespace hap {

shared_ptr<Value> IdentifierExpression::eval
  (Context& context, const shared_ptr<Environment> environment) const {
  return (*environment)[identifier];
}

bool IdentifierExpression::equal
  (const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const IdentifierExpression*>(&expression)) {
    return identifier == that->identifier;
  }
  return false;
}

void IdentifierExpression::write(ostream& stream) const {
  stream << identifier;
}

}
