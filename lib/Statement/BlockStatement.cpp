#include "BlockStatement.h"

#include "Value.h"
#include "UndefinedValue.h"
#include "indirect_equal.h"

#include <ostream>

using namespace std;

namespace hap {

BlockStatement::BlockStatement
  (std::initializer_list<Statement*> statements) {
  for (const auto& statement : statements)
    push(shared_ptr<Statement>(statement));
}

bool BlockStatement::equal(const Statement& statement) const {
  if (const auto that
      = dynamic_cast<const BlockStatement*>(&statement)) {
    return statements.size() == that->statements.size()
      && std::equal
        (begin(statements), end(statements),
         begin(that->statements), indirect_equal<Statement>());
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
