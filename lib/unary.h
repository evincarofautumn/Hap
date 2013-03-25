#ifndef HAP_UNARY_H
#define HAP_UNARY_H

#include "Operator.h"

#include <map>
#include <string>

namespace hap {

namespace unary {

extern std::map<std::string, Operator> operators;

Operator::Unary
  identity,
  negate,
  not_;

}

}

#endif
