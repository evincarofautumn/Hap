#include "Interpreter.h"

#include "Expression.h"
#include "Statement.h"
#include "Value.h"
#include "flow.h"

#include <iostream>

using namespace std;

namespace hap {

Interpreter::Interpreter(const shared_ptr<Environment> environment)
  : global_environment(environment) {}

void Interpreter::run(shared_ptr<Statement> statement) {
  try {
    statement->execute(global_context, global_environment);
  } catch (const flow::Exit&) {
  }
}

}
