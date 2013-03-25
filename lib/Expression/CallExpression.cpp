#include "CallExpression.h"

#include "FunExpression.h"
#include "Value.h"
#include "indirect_equal.h"

#include <algorithm>
#include <ostream>

using namespace std;

namespace hap {

CallExpression::CallExpression
  (shared_ptr<Expression> function,
   vector<shared_ptr<Expression>>&& expressions)
  : function(function),
    expressions(expressions) {}

shared_ptr<Value>CallExpression::eval
  (Context& context, const shared_ptr<Environment> environment) const {
  auto value(function->eval(context, environment));
  value->assert_type(Value::FUNCTION);
  const auto function(static_pointer_cast<FunExpression>(value));
  return function->call(context, expressions);
}

bool CallExpression::equal(const Expression& expression) const {
  if (auto other
      = dynamic_cast<const CallExpression*>(&expression)) {
    return *function == *other->function
      && expressions.size() == other->expressions.size()
      && std::equal
        (begin(expressions), end(expressions),
         begin(other->expressions), indirect_equal<Expression>());
  }
  return false;
}

void CallExpression::write(ostream& stream) const {
  stream << *function << "(";
  for (const auto& expression : expressions)
    stream << *expression << ", ";
  stream << ")";
}

}
