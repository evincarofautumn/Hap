#include "Statement.h"

#include "Context.h"
#include "Environment.h"
#include "Expression.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

Statement::~Statement() {}

void Statement::execute
  (Context& context,
   const std::shared_ptr<Environment> environment) const {
  exec(context, environment);
  context.interrupt(environment);
}

bool operator==(const Statement& a, const Statement& b) {
  return a.equal(b);
}

ostream& operator<<(ostream& stream, const Statement& statement) {
  statement.write(stream);
  return stream;
}

}
