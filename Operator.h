#ifndef HAP_OPERATOR_H
#define HAP_OPERATOR_H

#include <string>

namespace hap {

struct Operator {
  enum Arity {
    SENTINEL,
    UNARY,
    BINARY
  };
  enum Associativity {
    LEFT,
    RIGHT
  };
  Operator
    (Arity arity = SENTINEL,
     const std::string& operator_ = std::string(),
     Associativity associativity = LEFT,
     int precedence = 0)
    : arity(arity),
      operator_(operator_),
      associativity(associativity),
      precedence(precedence) {}
  Arity arity;
  std::string operator_;
  Associativity associativity;
  int precedence;
};

inline bool operator<(const Operator& a, const Operator& b) {
  return
    a.arity == Operator::BINARY && b.arity == Operator::BINARY
    ? a.precedence < b.precedence
      || (b.associativity == Operator::LEFT && a.precedence == b.precedence)
    : a.arity == Operator::BINARY && b.arity == Operator::UNARY
    ? !(b.precedence < a.precedence)
    : false;
}

}

#endif
