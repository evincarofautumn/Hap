#include "FunStatement.h"

#include "FunExpression.h"
#include "UndefinedValue.h"

#include <ostream>

using namespace std;

namespace hap {

FunStatement::FunStatement
  (const string& identifier,
   const vector<string>& parameters,
   shared_ptr<Statement> body,
   Environment& environment)
  : identifier(identifier),
    parameters(parameters),
    body(body),
    local(environment) {}

void FunStatement::exec(Environment& environment) const {
  unique_ptr<Value> value
    (new FunExpression(identifier, parameters, body, local));
  environment.define(identifier, move(value));
}

void FunStatement::write(ostream& stream) const {
  stream << "fun " << identifier << "(";
  for (const auto& parameter : parameters)
    stream << parameter << ", ";
  stream << ") ";
  body->write(stream);
}

}
