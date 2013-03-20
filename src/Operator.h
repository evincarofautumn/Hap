#ifndef HAP_OPERATOR_H
#define HAP_OPERATOR_H

#include <iosfwd>
#include <string>

namespace hap {

class Environment;
class Expression;
class Value;

class Operator {
private:
  typedef std::shared_ptr<Value> ValuePointer;
  typedef std::unique_ptr<const Expression> ExpressionPointer;
  typedef std::shared_ptr<Environment> EnvironmentPointer;
public:
  typedef ValuePointer Unary(EnvironmentPointer, const ExpressionPointer&);
  typedef ValuePointer Binary
    (EnvironmentPointer, const ExpressionPointer&, const ExpressionPointer&);
  enum Arity {
    SENTINEL,
    UNARY,
    BINARY
  };
  enum Associativity {
    LEFT,
    RIGHT
  };
  enum Precedence {
    COMMA,
    ASSIGNMENT,
    OR,
    XOR,
    AND,
    RELATIONAL,
    SHIFTING,
    ADDITIVE,
    MULTIPLICATIVE,
    TIGHTEST
  };
  Operator() : arity(SENTINEL) {}
  Operator
    (Arity arity,
     const std::string& operator_,
     Associativity associativity,
     Precedence precedence,
     Binary* binary)
    : arity(arity),
      operator_(operator_),
      associativity(associativity),
      precedence(precedence),
      unary(0),
      binary(binary) {}
  Operator
    (Arity arity,
     const std::string& operator_,
     Associativity associativity,
     Precedence precedence,
     Unary* unary)
    : arity(arity),
      operator_(operator_),
      associativity(associativity),
      precedence(precedence),
      unary(unary),
      binary(0) {}
  Arity arity;
  std::string operator_;
  Associativity associativity;
  Precedence precedence;
  Unary* unary;
  Binary* binary;
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

std::ostream& operator<<(std::ostream&, const Operator&);

}

#endif
