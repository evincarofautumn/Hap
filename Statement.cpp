#include "Statement.h"
#include "Environment.h"

#include <ostream>

using namespace std;

namespace hap {

Statement::~Statement() {}

ostream& operator<<(ostream& stream, const Statement& statement) {
  statement.write(stream);
  return stream;
}

void BlockStatement::exec(Environment& environment) const {
  for (const auto& statement : statements)
    statement->exec(environment);
}

void BlockStatement::write(ostream& stream) const {
  stream << "{\n";
  for (const auto& statement : statements)
    statement->write(stream);
  stream << "}\n";
}

void VarStatement::exec(Environment& environment) const {
  auto value = initializer
    ? initializer->eval(environment)
    : unique_ptr<Value>(new UndefinedExpression());
  environment.define(identifier, move(value));
}

void VarStatement::write(ostream& stream) const {
  stream << "var " << identifier;
  if (initializer) {
    stream << " = ";
    initializer->write(stream);
  }
  stream << ";\n";
}

void FlowStatement::exec(Environment& environment) const {
}

void FlowStatement::write(ostream& stream) const {
  stream << keyword << ' ';
  expression->write(stream);
  stream << '\n';
  statement->write(stream);
}

}
