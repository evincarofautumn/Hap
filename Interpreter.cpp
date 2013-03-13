#include "Interpreter.h"
#include "Expression.h"
#include "Statement.h"

#include <iostream>

using namespace std;

namespace hap {

void Interpreter::run(unique_ptr<Statement> statement) {
  auto result(statement->exec(global));
  cout << *result << '\n';
}

}
