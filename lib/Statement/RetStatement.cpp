#include "RetStatement.h"
#include "Expression.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

RetStatement::RetStatement(Expression* const expression)
  : expression(expression) {}

RetStatement::RetStatement(const shared_ptr<Expression> expression)
  : expression(expression) {}

bool RetStatement::equal(const Statement& statement) const {
  if (const auto that
      = dynamic_cast<const RetStatement*>(&statement)) {
    return *expression == *that->expression;
  }
  return false;
}

void RetStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  auto value(expression->eval(context, environment));
  throw flow::Return(value);
}

void RetStatement::write(ostream& stream) const {
  stream << "ret " << *expression << ";\n";
}

}
