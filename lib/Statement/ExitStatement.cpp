#include "ExitStatement.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

bool ExitStatement::equal(const Statement& statement) const {
  return dynamic_cast<const ExitStatement*>(&statement);
}

void ExitStatement::exec(Context&, const shared_ptr<Environment>) const {
  throw flow::Exit();
}

void ExitStatement::write(ostream& stream) const {
  stream << "exit;\n";
}

}
