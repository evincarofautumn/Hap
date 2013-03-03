#ifndef HAP_EXPRESSION_H
#define HAP_EXPRESSION_H

#include "Operator.h"

#include <memory>
#include <string>
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
  void push(std::unique_ptr<Expression> expression) {
    expressions.push_back(std::move(expression));
  }
private:
  std::vector< std::unique_ptr<Expression> > expressions;
};

class BinaryExpression : public Expression {
public:
  BinaryExpression
    (const Operator& operator_,
     std::unique_ptr<Expression> a,
     std::unique_ptr<Expression> b)
    : operator_(operator_), a(std::move(a)), b(std::move(b)) {}
private:
  Operator operator_;
  std::unique_ptr<Expression> a;
  std::unique_ptr<Expression> b;
};

class UnaryExpression : public Expression {
public:
  UnaryExpression
    (const Operator& operator_,
     std::unique_ptr<Expression> a)
    : operator_(operator_), a(std::move(a)) {}
private:
  Operator operator_;
  std::unique_ptr<Expression> a;
};

}

#endif
