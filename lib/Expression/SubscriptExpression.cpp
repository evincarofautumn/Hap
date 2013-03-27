#include "SubscriptExpression.h"

#include "IntegerValue.h"
#include "ListValue.h"
#include "Value.h"

#include <ostream>

using namespace std;

namespace hap {

SubscriptExpression::SubscriptExpression
  (Expression* const expression, Expression* const subscript)
  : expression(expression),
    subscript(subscript) {}

SubscriptExpression::SubscriptExpression
  (shared_ptr<Expression> expression,
   shared_ptr<Expression> subscript)
  : expression(expression),
    subscript(subscript) {}

shared_ptr<Value> SubscriptExpression::eval
  (Context& context, const shared_ptr<Environment> environment) const {
  const auto value(eval_as<Value::LIST>(expression, context, environment));
  const auto index(eval_as<Value::INTEGER>(subscript, context, environment));
  return (*value)[index->value];
}

bool SubscriptExpression::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const SubscriptExpression*>(&expression)) {
    return *this->expression == *that->expression
      && *subscript == *that->subscript;
  }
  return false;
}

void SubscriptExpression::write(ostream& stream) const {
  stream << *expression << "[" << *subscript << "]";
}

}
