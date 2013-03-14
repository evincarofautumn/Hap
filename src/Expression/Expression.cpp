#include "Expression.h"
#include "Environment.h"
#include "flow.h"
#include "unique_cast.h"

#include <ostream>
#include <sstream>

using namespace std;

namespace hap {

Expression::~Expression() {}

ostream& operator<<(ostream& stream, const Expression& expression) {
  expression.write(stream);
  return stream;
}

}
