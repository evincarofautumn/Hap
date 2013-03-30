#include "DelStatement.h"

#include "Environment.h"

#include <ostream>

using namespace std;

namespace hap {

DelStatement::DelStatement(const std::string& identifier)
  : identifier(identifier) {}

bool DelStatement::equal(const Statement& statement) const {
  if (const auto that
      = dynamic_cast<const DelStatement*>(&statement)) {
    return identifier == that->identifier;
  }
  return false;
}

void DelStatement::exec
  (Context&, const shared_ptr<Environment> environment) const {
  environment->undefine(identifier);
}

void DelStatement::write(ostream& stream) const {
  stream << "del " << identifier << ";\n";
}

}
