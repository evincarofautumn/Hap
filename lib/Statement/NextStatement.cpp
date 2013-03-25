#include "NextStatement.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

bool NextStatement::equal(const Statement& statement) const {
  return dynamic_cast<const NextStatement*>(&statement);
}

void NextStatement::exec(Context&, const shared_ptr<Environment>) const {
  throw flow::Next();
}

void NextStatement::write(ostream& stream) const {
  stream << "next;\n";
}

}
