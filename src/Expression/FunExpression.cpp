#include "FunExpression.h"

#include "UndefinedValue.h"
#include "flow.h"

#include <iostream>

using namespace std;

namespace hap {

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
  (const std::shared_ptr<Environment> environment) const {
  throw runtime_error("unimplemented fun");
}

bool FunExpression::less(const Value& other) const {
  return Value::less(other);
}

void FunExpression::write(ostream& stream) const {
  stream << "\\" << identifier << "(";
  for (const auto& parameter : parameters)
    stream << parameter << ", ";
  stream << ") ";
  body->write(stream);
}

shared_ptr<Value> FunExpression::call
  (const vector<unique_ptr<Expression>>& arguments) const {
  shared_ptr<Environment> local(new Environment(environment));
  auto parameter(parameters.begin());
  for (const auto& argument : arguments) {
    auto value(argument->eval(environment));
    local->define(*parameter++, value);
  }
  try {
    body->exec(local);
  } catch (flow::Return& result) {
    return result.value;
  }
  return unique_ptr<Value>(new UndefinedValue());
}

}
