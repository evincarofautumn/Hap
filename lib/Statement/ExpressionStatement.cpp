#include "ExpressionStatement.h"
#include "Expression.h"
#include "Value.h"

#include <ostream>

using namespace std;

namespace hap {

ExpressionStatement::ExpressionStatement
  (shared_ptr<Expression> expression)
  : expression(expression) {}

bool ExpressionStatement::equal(const Statement& statement) const {
  if (const auto that
      = dynamic_cast<const ExpressionStatement*>(&statement)) {
    return *expression == *that->expression;
  }
  return false;
}

void ExpressionStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  expression->eval(context, environment);
}

void ExpressionStatement::write(ostream& stream) const {
  stream << *expression << ";\n";
}

}
