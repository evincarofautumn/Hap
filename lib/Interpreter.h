#ifndef HAP_INTERPRETER_H
#define HAP_INTERPRETER_H

#include "Context.h"
#include "Environment.h"

#include <memory>

namespace hap {

class Statement;

class Interpreter {
public:
  Interpreter(std::shared_ptr<Environment>);
  void run(std::shared_ptr<Statement>);
private:
  Context global_context;
  std::shared_ptr<Environment> global_environment;
};

}

#endif
