#include "AtomicStatement.h"

#include "Atomic.h"

#include <ostream>

using namespace std;

namespace hap {

AtomicStatement::AtomicStatement
  (const shared_ptr<Statement> statement)
  : statement(statement) {}

bool AtomicStatement::equal(const Statement& statement) const {
  if (auto other
      = dynamic_cast<const AtomicStatement*>(&statement)) {
    return *this->statement == *other->statement;
  }
  return false;
}

void AtomicStatement::exec
  (Context& context,
   const shared_ptr<Environment> environment) const {
  Atomic atomic(context);
  statement->execute(context, environment);
}

void AtomicStatement::write(ostream& stream) const {
  stream << "atomic " << *statement;
}

}
