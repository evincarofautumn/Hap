#include "ForStatement.h"

#include "BooleanValue.h"
#include "Environment.h"
#include "flow.h"

#include <ostream>

using namespace std;

namespace hap {

ForStatement::ForStatement
  (Statement* const initializer,
   Expression* const condition,
   Expression* const step,
   Statement* const body)
  : initializer(initializer),
    condition(condition),
    step(step),
    body(body) {}

ForStatement::ForStatement
  (const shared_ptr<Statement> initializer,
   const shared_ptr<Expression> condition,
   const shared_ptr<Expression> step,
   const shared_ptr<Statement> body)
  : initializer(initializer),
    condition(condition),
    step(step),
    body(body) {}

bool ForStatement::equal(const Statement& statement) const {
  if (const auto that
      = dynamic_cast<const ForStatement*>(&statement)) {
    return *initializer == *that->initializer
      && *condition == *that->condition
      && *step == *that->step
      && *body == *that->body;
  }
  return false;
}

void ForStatement::exec
  (Context& context, const shared_ptr<Environment> environment) const {
  const auto local(shared_ptr<Environment>(new Environment(environment)));
  for (initializer->execute(context, local);
    eval_as<Value::BOOLEAN>(condition, context, local)->value;
    step->eval(context, local)) try {
    body->execute(context, local);
  } catch (const flow::Last&) {
    break;
  } catch (const flow::Next&) {
    continue;
  }
}

void ForStatement::write(ostream& stream) const {
  stream
    << "for ("
    << *initializer << "; "
    << *condition << "; "
    << *step << ")\n"
    << *body;
}

}