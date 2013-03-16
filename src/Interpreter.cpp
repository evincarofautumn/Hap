#include "Interpreter.h"
#include "Expression.h"
#include "Statement.h"
#include "Value.h"

#include <iostream>

using namespace std;

namespace hap {

void Interpreter::run(unique_ptr<Statement> statement) {
  statement->exec(global);
}

}
