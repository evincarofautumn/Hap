#include "BlockStatement.h"
#include "Value.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> BlockStatement::exec(Environment& environment) const {
  unique_ptr<Value> result;
  for (const auto& statement : statements)
    result = statement->exec(environment);
  return move(result);
}

void BlockStatement::write(ostream& stream) const {
  stream << "{\n";
  for (const auto& statement : statements)
    statement->write(stream);
  stream << "}\n";
}

}
