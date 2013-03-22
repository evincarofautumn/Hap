#include "LastStatement.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

void LastStatement::exec(Context&, const shared_ptr<Environment>) const {
  throw flow::Last();
}

void LastStatement::write(ostream& stream) const {
  stream << "last;\n";
}

}
