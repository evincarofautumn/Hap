#include "BlockStatement.h"

#include "Value.h"
#include "UndefinedValue.h"
#include "indirect_equal.h"

#include <ostream>

using namespace std;

namespace hap {

bool BlockStatement::equal(const Statement& statement) const {
  if (auto other
      = dynamic_cast<const BlockStatement*>(&statement)) {
    return statements.size() == other->statements.size()
      && std::equal
        (begin(statements), end(statements),
         begin(other->statements), indirect_equal<Statement>());
  }
  return false;
}

void BlockStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  for (const auto& statement : statements)
    statement->execute(context, environment);
}

void BlockStatement::write(ostream& stream) const {
  stream << "{\n";
  for (const auto& statement : statements)
    stream << *statement;
  stream << "}\n";
}

}
