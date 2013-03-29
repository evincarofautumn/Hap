#include "FunExpression.h"

#include "UndefinedValue.h"
#include "flow.h"
#include "indirect_equal.h"

#include <iostream>

using namespace std;

namespace hap {

FunExpression::FunExpression
  (const std::string& identifier,
   std::initializer_list<std::string> parameters,
   Statement* body)
  : identifier(identifier),
    parameters(parameters),
    body(body) {}

FunExpression::FunExpression
  (const string& identifier,
   const vector<string>& parameters,
   shared_ptr<Statement> body,
   shared_ptr<Environment> environment)
  : identifier(identifier),
    parameters(parameters),
    body(body),
    environment(environment) {}

shared_ptr<Value> FunExpression::eval
  (Context&, const shared_ptr<Environment> environment) const {
  return shared_ptr<Value>(new FunExpression(*this));
}

bool FunExpression::less(const Value& that) const {
  return Value::less(that);
}

bool FunExpression::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const FunExpression*>(&expression)) {
    return identifier == that->identifier
      && parameters == that->parameters
      && *body == *that->body;
  }
  return false;
}

void FunExpression::write(ostream& stream) const {
  stream << "lam " << identifier << "(";
  for (const auto& parameter : parameters)
    stream << parameter << ", ";
  stream << ") " << *body;
}

shared_ptr<Value> FunExpression::call
  (Context& context, const vector<shared_ptr<Expression>>& arguments) const {
  shared_ptr<Environment> local(new Environment(environment));
  auto parameter(parameters.begin());
  for (const auto& argument : arguments) {
    auto value(argument->eval(context, environment));
    local->define(*parameter++, value);
  }
  try {
    body->execute(context, local);
  } catch (flow::Return& result) {
    return result.value;
  }
  return shared_ptr<Value>(new UndefinedValue());
}

}
