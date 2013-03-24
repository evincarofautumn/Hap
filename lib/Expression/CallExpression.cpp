#include "CallExpression.h"
#include "FunExpression.h"
#include "Value.h"

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

void CallExpression::write(ostream& stream) const {
  function->write(stream);
  stream << "(";
  for (const auto& expression : expressions) {
    expression->write(stream);
    stream << ", ";
  }
  stream << ")";
}

}
