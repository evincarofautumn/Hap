#include "FlowStatement.h"

#include "BooleanValue.h"
#include "Context.h"
#include "UndefinedValue.h"
#include "Value.h"
#include "flow.h"
#include "unique_cast.h"

#include <ostream>

using namespace std;

namespace hap {

FlowStatement::FlowStatement
  (const string& keyword,
   shared_ptr<Expression> expression,
   shared_ptr<Statement> statement)
  : keyword(keyword),
    expression(expression),
    statement(statement) {}

void FlowStatement::write(ostream& stream) const {
  stream << keyword << ' ';
  expression->write(stream);
  stream << '\n';
  statement->write(stream);
}

#define FLOW_STATEMENT(NAME, KEYWORD) \
  NAME##Statement::NAME##Statement \
    (shared_ptr<Expression> expression, \
     shared_ptr<Statement> statement) \
    : FlowStatement(KEYWORD, expression, statement) {}

FLOW_STATEMENT(If, "if")
FLOW_STATEMENT(When, "when")
FLOW_STATEMENT(Whenever, "whenever")
FLOW_STATEMENT(While, "while")
FLOW_STATEMENT(RepeatWhen, "repeat_when")
FLOW_STATEMENT(RepeatWhenever, "repeat_whenever")

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

void RepeatWhenStatement::exec
  (Context&, const shared_ptr<Environment>) const {
  throw runtime_error("unimplemented repeat_when");
}

void RepeatWheneverStatement::exec
  (Context&, const shared_ptr<Environment>) const {
  throw runtime_error("unimplemented repeat_whenever");
}

}
