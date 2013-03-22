#include "Interpreter.h"
#include "Expression.h"
#include "Statement.h"
#include "Value.h"

#include <iostream>

using namespace std;

namespace hap {

Interpreter::Interpreter(const shared_ptr<Environment> environment)
  : global_environment(environment) {}

void Interpreter::run(shared_ptr<Statement> statement) {
  statement->exec(global_context, global_environment);
}

}
