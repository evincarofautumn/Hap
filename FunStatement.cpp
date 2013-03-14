#include "FunStatement.h"
#include "FunExpression.h"
#include "UndefinedExpression.h"

#include <ostream>

using namespace std;

namespace hap {

FunStatement::FunStatement
  (const string& identifier,
   const vector<string>& parameters,
   shared_ptr<Statement> body,
   const Environment& environment)
  : identifier(identifier),
    parameters(parameters),
    body(body),
    local(environment) {}

unique_ptr<Value> FunStatement::exec(Environment& environment) const {
  unique_ptr<Value> value
    (new FunExpression(identifier, parameters, body, local));
  environment.define(identifier, move(value));
  return unique_ptr<Value>(new UndefinedExpression());
}

void FunStatement::write(ostream& stream) const {
  stream << "fun " << identifier << "(";
  for (const auto& parameter : parameters)
    stream << parameter << ", ";
  stream << ") ";
  body->write(stream);
}

}
