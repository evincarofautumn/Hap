#ifndef HAP_EXPRESSION_H
#define HAP_EXPRESSION_H

#include "Operator.h"

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace hap {

class BinaryOperator;
class UnaryOperator;

class Expression {
public:
  virtual ~Expression();
  virtual void write(std::ostream&) const = 0;
};

std::ostream& operator<<(std::ostream&, const Expression&);

class IntegerExpression : public Expression {
public:
  IntegerExpression(int value)
    : value(value) {}
  virtual void write(std::ostream&) const override;
private:
  int value;
};

class IdentifierExpression : public Expression {
public:
  IdentifierExpression(const std::string& identifier)
    : identifier(identifier) {}
  virtual void write(std::ostream&) const override;
private:
  std::string identifier;
};

class ListExpression : public Expression {
public:
  ListExpression() {}
  void push(std::unique_ptr<Expression> expression) {
    expressions.push_back(std::move(expression));
  }
  virtual void write(std::ostream&) const override;
private:
  std::vector<std::unique_ptr<Expression>> expressions;
};

class BinaryExpression : public Expression {
public:
  BinaryExpression
    (const Operator& operator_,
     std::unique_ptr<Expression> a,
     std::unique_ptr<Expression> b)
    : operator_(operator_), a(std::move(a)), b(std::move(b)) {}
  virtual void write(std::ostream&) const override;
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
  virtual void write(std::ostream&) const override;
private:
  Operator operator_;
  std::unique_ptr<Expression> a;
};

}

#endif
