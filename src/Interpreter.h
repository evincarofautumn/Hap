#ifndef HAP_INTERPRETER_H
#define HAP_INTERPRETER_H

#include "Environment.h"
#include <memory>

namespace hap {

class Statement;

class Interpreter {
public:
  Interpreter(std::shared_ptr<Environment>);
  void run(std::unique_ptr<Statement>);
private:
  std::shared_ptr<Environment> global;
};

}

#endif
