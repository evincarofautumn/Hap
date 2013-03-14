#ifndef HAP_BINARY_H
#define HAP_BINARY_H

#include "Operator.h"

namespace hap {

namespace binary {

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
