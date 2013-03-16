#include "VarStatement.h"

#include "UndefinedExpression.h"
#include "Environment.h"

#include <ostream>

using namespace std;

namespace hap {

VarStatement::VarStatement
  (const string& identifier,
   unique_ptr<Expression> initializer)
  : identifier(identifier),
    initializer(move(initializer)) {}

void VarStatement::exec(Environment& environment) const {
  auto value(initializer->eval(environment));
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

}
