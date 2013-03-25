#include "FlowStatement.h"

#include "BooleanValue.h"
#include "Context.h"
#include "UndefinedValue.h"
#include "Value.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

FlowStatement::FlowStatement
  (const string& keyword,
   const shared_ptr<Expression> expression,
   const shared_ptr<Statement> statement)
  : keyword(keyword),
    expression(expression),
    statement(statement) {}

bool FlowStatement::equal(const Statement& statement) const {
  if (auto other
      = dynamic_cast<const FlowStatement*>(&statement)) {
    return keyword == other->keyword
      && *expression == *other->expression
      && *this->statement == *other->statement;
  }
  return false;
}

void FlowStatement::write(ostream& stream) const {
  stream << keyword << ' ' << *expression << '\n' << *statement;
}

#define FLOW_STATEMENT(NAME, KEYWORD) \
  NAME##Statement::NAME##Statement \
    (const shared_ptr<Expression> expression, \
     const shared_ptr<Statement> statement) \
    : FlowStatement(KEYWORD, expression, statement) {}

FLOW_STATEMENT(If, "if")
FLOW_STATEMENT(When, "when")
FLOW_STATEMENT(Whenever, "whenever")
FLOW_STATEMENT(While, "while")

#undef FLOW_STATEMENT

void IfStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  if (eval_as<Value::BOOLEAN>(expression, context, environment)->value)
    statement->execute(context, environment);
}

void WhenStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  context.listen(expression, Context::ONCE, statement, environment);
}

void WheneverStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  context.listen(expression, Context::REPEATEDLY, statement, environment);
}

void WhileStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  while (true) {
    if (!eval_as<Value::BOOLEAN>(expression, context, environment)->value)
      break;
    try {
      statement->execute(context, environment);
    } catch (const flow::Last&) {
      break;
    } catch (const flow::Next&) {
      continue;
    }
  }
}

}
