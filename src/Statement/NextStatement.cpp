#include "NextStatement.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

void NextStatement::exec(Environment&) const {
  throw flow::Next();
}

void NextStatement::write(ostream& stream) const {
  stream << "next;\n";
}

}