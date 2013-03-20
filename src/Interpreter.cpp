#include "Interpreter.h"
#include "Expression.h"
#include "Statement.h"
#include "Value.h"

#include <iostream>

using namespace std;

namespace hap {

Interpreter::Interpreter(const shared_ptr<Environment> global)
  : global(global) {}

void Interpreter::run(unique_ptr<Statement> statement) {
  statement->exec(global);
}

}
