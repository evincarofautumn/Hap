#include "FunStatement.h"

#include "FunExpression.h"
#include "UndefinedValue.h"

#include <ostream>

using namespace std;

namespace hap {

FunStatement::FunStatement
  (const string& identifier,
   const vector<string>& parameters,
   const shared_ptr<Statement> body,
   const std::shared_ptr<Environment> environment)
  : identifier(identifier),
    parameters(parameters),
    body(body),
    local(environment) {}

void FunStatement::exec
  (const std::shared_ptr<Environment> environment) const {
  environment->define(identifier, shared_ptr<Value>
    (new FunExpression(identifier, parameters, body, local)));
}

void FunStatement::write(ostream& stream) const {
  stream << "fun " << identifier << "(";
  for (const auto& parameter : parameters)
    stream << parameter << ", ";
  stream << ") ";
  body->write(stream);
}

}
