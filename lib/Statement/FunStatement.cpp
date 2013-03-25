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
   const shared_ptr<Environment> environment)
  : identifier(identifier),
    parameters(parameters),
    body(body),
    local(environment) {}

bool FunStatement::equal(const Statement& statement) const {
  if (const auto that
      = dynamic_cast<const FunStatement*>(&statement)) {
    return identifier == that->identifier
      && parameters == that->parameters
      && *body == *that->body;
  }
  return false;
}

void FunStatement::exec
  (Context&, const shared_ptr<Environment> environment) const {
  environment->define(identifier, shared_ptr<Value>
    (new FunExpression(identifier, parameters, body, local)));
}

void FunStatement::write(ostream& stream) const {
  stream << "fun " << identifier << "(";
  for (const auto& parameter : parameters)
    stream << parameter << ", ";
  stream << ") " << *body;
}

}
