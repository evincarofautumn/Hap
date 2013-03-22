#include "TraceStatement.h"
#include "Value.h"

#include <iostream>

using namespace std;

namespace hap {

TraceStatement::TraceStatement(shared_ptr<Expression> expression)
  : expression(expression) {}

void TraceStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  auto value(expression->eval(context, environment));
  value->write(cout);
  cout << endl;
}

void TraceStatement::write(ostream& stream) const {
  stream << "trace ";
  expression->write(stream);
  stream << ";\n";
}

}
