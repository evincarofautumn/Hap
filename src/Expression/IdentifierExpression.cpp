#include "IdentifierExpression.h"

#include "Value.h"

using namespace std;

namespace hap {

unique_ptr<Value> IdentifierExpression::eval(Environment& environment) const {
  return unique_ptr<Value>(environment[identifier].copy());
}

void IdentifierExpression::write(ostream& stream) const {
  stream << identifier;
}

}
