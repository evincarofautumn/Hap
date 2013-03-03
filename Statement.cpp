#include "Statement.h"

#include <ostream>

using namespace std;

namespace hap {

Statement::~Statement() {}

ostream& operator<<(ostream& stream, const Statement& statement) {
  statement.write(stream);
  return stream;
}

void BlockStatement::write(ostream& stream) const {
  stream << "{\n";
  for (const auto& statement : statements)
    statement->write(stream);
  stream << "}\n";
}

void VarStatement::write(ostream& stream) const {
  stream << "var " << identifier;
  if (initializer) {
    stream << " = ";
    initializer->write(stream);
  }
  stream << ";\n";
}

void FlowStatement::write(ostream& stream) const {
  stream << keyword << ' ';
  expression->write(stream);
  stream << '\n';
  statement->write(stream);
}

}
