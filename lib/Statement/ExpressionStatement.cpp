#include "ExpressionStatement.h"
#include "Expression.h"
#include "Value.h"

#include <ostream>

using namespace std;

namespace hap {

ExpressionStatement::ExpressionStatement
  (shared_ptr<Expression> expression)
  : expression(expression) {}

void ExpressionStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  expression->eval(context, environment);
}

void ExpressionStatement::write(ostream& stream) const {
  expression->write(stream);
  stream << ";\n";
}

}
