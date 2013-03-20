#include "BlockStatement.h"

#include "Value.h"
#include "UndefinedValue.h"

#include <ostream>

using namespace std;

namespace hap {

void BlockStatement::exec
  (const std::shared_ptr<Environment> environment) const {
  for (const auto& statement : statements)
    statement->exec(environment);
}

void BlockStatement::write(ostream& stream) const {
  stream << "{\n";
  for (const auto& statement : statements)
    statement->write(stream);
  stream << "}\n";
}

}
