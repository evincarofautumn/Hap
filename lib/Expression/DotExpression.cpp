#include "DotExpression.h"

#include "MapValue.h"
#include "StringValue.h"

#include <ostream>

using namespace std;

namespace hap {

DotExpression::DotExpression
  (Expression* const expression,
   const string& key)
  : expression(expression),
    key(key) {}

DotExpression::DotExpression
  (shared_ptr<Expression> expression,
   const string& key)
  : expression(expression),
    key(key) {}

shared_ptr<Value> DotExpression::eval
  (Context& context, const shared_ptr<Environment> environment) const {
  const auto map(eval_as<Value::MAP>(expression, context, environment));
  return (*map)[make_shared<StringValue>(key)];
}

bool DotExpression::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const DotExpression*>(&expression)) {
    return *this->expression == *that->expression
      && key == that->key;
  }
  return false;
}

void DotExpression::write(ostream& stream) const {
  stream << *expression << "." << key;
}

}
