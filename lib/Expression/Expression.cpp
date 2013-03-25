#include "Expression.h"
#include "Environment.h"
#include "flow.h"

#include <ostream>
#include <sstream>

using namespace std;

namespace hap {

Expression::~Expression() {}

bool operator==(const Expression& a, const Expression& b) {
  return a.equal(b);
}

ostream& operator<<(ostream& stream, const Expression& expression) {
  expression.write(stream);
  return stream;
}

}
