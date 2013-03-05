#ifndef HAP_INTERPRETER_H
#define HAP_INTERPRETER_H

#include "Environment.h"
#include <memory>

namespace hap {

class Statement;

class Interpreter {
public:
  void run(std::unique_ptr<Statement>);
private:
  Environment global;
};

}

#endif
