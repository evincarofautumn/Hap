#include "TraceStatement.h"
#include "Value.h"

#include <iostream>

using namespace std;

namespace hap {

TraceStatement::TraceStatement(shared_ptr<Expression> expression)
  : expression(expression) {}

bool TraceStatement::equal(const Statement& statement) const {
  if (auto other
      = dynamic_cast<const TraceStatement*>(&statement)) {
    return *expression == *other->expression;
  }
  return false;
}

void TraceStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  auto value(expression->eval(context, environment));
  cout << *value << endl;
}

void TraceStatement::write(ostream& stream) const {
  stream << "trace " << *expression << ";\n";
}

}
