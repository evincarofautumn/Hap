#include "RetStatement.h"
#include "Expression.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

RetStatement::RetStatement(unique_ptr<Expression> expression)
  : expression(move(expression)) {}

void RetStatement::exec(const std::shared_ptr<Environment> environment) const {
  auto value(expression->eval(environment));
  throw flow::Return(value);
}

void RetStatement::write(ostream& stream) const {
  stream << "ret ";
  expression->write(stream);
  stream << ";\n";
}

}
