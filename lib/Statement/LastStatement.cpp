#include "LastStatement.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

bool LastStatement::equal(const Statement& statement) const {
  return dynamic_cast<const LastStatement*>(&statement);
}

void LastStatement::exec(Context&, const shared_ptr<Environment>) const {
  throw flow::Last();
}

void LastStatement::write(ostream& stream) const {
  stream << "last;\n";
}

}
