#include "CallExpression.h"

#include "FunValue.h"
#include "Value.h"
#include "indirect_compare.h"

#include <algorithm>
#include <ostream>

using namespace std;

namespace hap {

CallExpression::CallExpression
  (Expression* const function,
   std::initializer_list<Expression*> expressions)
  : function(function) {
  for (const auto& expression : expressions)
    this->expressions.push_back(shared_ptr<Expression>(expression));
}

CallExpression::CallExpression
  (shared_ptr<Expression> function,
   vector<shared_ptr<Expression>>&& expressions)
  : function(function),
    expressions(expressions) {}

shared_ptr<Value> CallExpression::eval
  (Context& context, const shared_ptr<Environment> environment) const {
  const auto value
    (eval_as<Value::FUNCTION>(function, context, environment));
  return value->call(context, expressions);
}

bool CallExpression::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const CallExpression*>(&expression)) {
    return *function == *that->function
      && expressions.size() == that->expressions.size()
      && std::equal
        (begin(expressions), end(expressions),
         begin(that->expressions), indirect_equal<Expression>());
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
