#include "TraceStatement.h"
#include "Value.h"

#include <iostream>

using namespace std;

namespace hap {

TraceStatement::TraceStatement(unique_ptr<Expression> expression)
  : expression(move(expression)) {}

void TraceStatement::exec(Environment& environment) const {
  auto value(expression->eval(environment));
  value->write(cout);
  cout << endl;
}

void TraceStatement::write(ostream& stream) const {
  stream << "trace ";
  expression->write(stream);
  stream << ";\n";
}

}
