#ifndef HAP_BINARY_H
#define HAP_BINARY_H

#include "Operator.h"

#include <map>
#include <string>

namespace hap {

namespace binary {

extern std::map<std::string, Operator> operators;

Operator::Binary
  multiply,
  divide,
  modulate,
  add,
  subtract,
  shift_left,
  shift_right,
  lt,
  ge,
  gt,
  le,
  eq,
  ne,
  and_,
  xor_,
  or_,
  assign,
  comma;

}

}

#endif
