#include "ExpressionStatement.h"
#include "Expression.h"
#include "Value.h"

#include <ostream>

using namespace std;

namespace hap {

ExpressionStatement::ExpressionStatement
  (unique_ptr<Expression> expression)
  : expression(move(expression)) {}

void ExpressionStatement::exec
  (const shared_ptr<Environment> environment) const {
  expression->eval(environment);
}

void ExpressionStatement::write(ostream& stream) const {
  expression->write(stream);
  stream << ";\n";
}

}
