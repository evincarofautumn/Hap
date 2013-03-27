#include "SubscriptExpression.h"

#include "IntegerValue.h"
#include "ListValue.h"
#include "MapValue.h"
#include "Value.h"

#include <ostream>
#include <sstream>

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
  const auto value(expression->eval(context, environment));
  const auto key(subscript->eval(context, environment));
  if (const auto list = dynamic_cast<const ListValue*>(value.get())) {
    key->assert_type(Value::INTEGER);
    return (*list)[static_cast<const IntegerValue*>(key.get())->value];
  }
  if (const auto map = dynamic_cast<MapValue*>(value.get()))
    return (*map)[key];
  ostringstream message;
  message << "expected list or map but got " << value->type();
  throw runtime_error(message.str());
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
