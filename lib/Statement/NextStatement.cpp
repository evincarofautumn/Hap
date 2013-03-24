#include "NextStatement.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

void NextStatement::exec(Context&, const shared_ptr<Environment>) const {
  throw flow::Next();
}

void NextStatement::write(ostream& stream) const {
  stream << "next;\n";
}

}
