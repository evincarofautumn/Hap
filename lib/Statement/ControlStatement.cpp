#include "ControlStatement.h"

#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

bool ControlStatement::equal(const Statement& statement) const {
  if (const auto that
      = dynamic_cast<const ControlStatement*>(&statement)) {
    return control == that->control;
  }
  return false;
}

void ControlStatement::write(ostream& stream) const {
  stream << control << ";\n";
}

#define CONTROL_STATEMENT(NAME) \
void NAME##Statement::exec(Context&, const shared_ptr<Environment>) const { \
  throw flow::NAME(); \
}

CONTROL_STATEMENT(Exit)
CONTROL_STATEMENT(Next)
CONTROL_STATEMENT(Last)

#undef CONTROL_STATEMENT

ostream& operator<<(ostream& stream, ControlStatement::Control control) {
  switch (control) {
  case ControlStatement::EXIT: return stream << "exit";
  case ControlStatement::NEXT: return stream << "next";
  case ControlStatement::LAST: return stream << "last";
  }
}

}
