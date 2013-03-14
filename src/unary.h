#ifndef HAP_UNARY_H
#define HAP_UNARY_H

#include "Operator.h"

namespace hap {

namespace unary {

Operator::Unary
  identity,
  negate,
  val,
  ref,
  not_,
  lt,
  ge,
  gt,
  le,
  eq,
  ne;

}

}

#endif
