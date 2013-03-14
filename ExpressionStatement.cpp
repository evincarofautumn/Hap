#include "ExpressionStatement.h"
#include "Expression.h"
#include "Value.h"

#include <ostream>

using namespace std;

namespace hap {

ExpressionStatement::ExpressionStatement
  (unique_ptr<Expression> expression)
  : expression(move(expression)) {}

unique_ptr<Value> ExpressionStatement::exec(Environment& environment) const {
  return expression->eval(environment);
}

void ExpressionStatement::write(ostream& stream) const {
  expression->write(stream);
  stream << ";\n";
}

}
