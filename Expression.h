#ifndef HAP_EXPRESSION_H
#define HAP_EXPRESSION_H

#include "Operator.h"

#include <string>
#include <tr1/memory>
#include <vector>

namespace hap {

class BinaryOperator;
class UnaryOperator;

class Expression {};

class IntegerExpression : public Expression {
public:
  IntegerExpression(int value)
    : value(value) {}
private:
  int value;
};

class IdentifierExpression : public Expression {
public:
  IdentifierExpression(const std::string& identifier)
    : identifier(identifier) {}
private:
  std::string identifier;
};

class ListExpression : public Expression {
public:
  ListExpression() {}
  void push(std::tr1::shared_ptr<Expression> const expression) {
    expressions.push_back(expression);
  }
private:
  std::vector< std::tr1::shared_ptr<Expression> > expressions;
};

class BinaryExpression : public Expression {
public:
  BinaryExpression
    (const Operator& operator_,
     const std::tr1::shared_ptr<Expression> a,
     const std::tr1::shared_ptr<Expression> b)
    : operator_(operator_), a(a), b(b) {}
private:
  Operator operator_;
  std::tr1::shared_ptr<Expression> a;
  std::tr1::shared_ptr<Expression> b;
};

class UnaryExpression : public Expression {
public:
  UnaryExpression
    (const Operator& operator_,
     const std::tr1::shared_ptr<Expression> a)
    : operator_(operator_), a(a) {}
private:
  Operator operator_;
  std::tr1::shared_ptr<Expression> a;
};

}

#endif
