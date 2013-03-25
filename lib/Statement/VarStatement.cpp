#include "VarStatement.h"

#include "UndefinedValue.h"
#include "Environment.h"

#include <ostream>

using namespace std;

namespace hap {

VarStatement::VarStatement
  (const string& identifier,
   shared_ptr<Expression> initializer)
  : identifier(identifier),
    initializer(initializer) {}

bool VarStatement::equal(const Statement& statement) const {
  if (auto other
      = dynamic_cast<const VarStatement*>(&statement)) {
    return identifier == other->identifier
      && *initializer == *other->initializer;
  }
  return false;
}

void VarStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  auto value(initializer->eval(context, environment));
  environment->define(identifier, value);
}

void VarStatement::write(ostream& stream) const {
  stream << "var " << identifier;
  if (initializer)
    stream << " = " << *initializer;
  stream << ";\n";
}

}
