#include "VarStatement.h"
#include "UndefinedExpression.h"

#include <ostream>

using namespace std;

namespace hap {

VarStatement::VarStatement
  (const string& identifier,
   unique_ptr<Expression> initializer)
  : identifier(identifier),
    initializer(move(initializer)) {}

unique_ptr<Value> VarStatement::exec(Environment& environment) const {
  auto value = initializer
    ? initializer->eval(environment)
    : unique_ptr<Value>(new UndefinedExpression());
  environment.define(identifier, move(value));
  return unique_ptr<Value>(new UndefinedExpression());
}

void VarStatement::write(ostream& stream) const {
  stream << "var " << identifier;
  if (initializer) {
    stream << " = ";
    initializer->write(stream);
  }
  stream << ";\n";
}

}
