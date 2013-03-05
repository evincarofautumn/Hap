#ifndef HAP_EXPRESSION_H
#define HAP_EXPRESSION_H

#include "Operator.h"

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace hap {

class BinaryOperator;
class Environment;
class UnaryOperator;

class Value {
public:
  enum Type {
    UNDEFINED,
    INTEGER,
    LIST,
  };
  void assert_type(Type) const;
  virtual ~Value();
  virtual Value* copy() const = 0;
  virtual Type type() const = 0;
};

std::ostream& operator<<(std::ostream&, const Value::Type&);

class Expression {
public:
  virtual ~Expression();
  virtual std::unique_ptr<Value> eval(Environment&) const = 0;
  virtual void write(std::ostream&) const = 0;
};

std::ostream& operator<<(std::ostream&, const Expression&);

class IntegerExpression : public Expression, public Value {
public:
  IntegerExpression(int value)
    : value(value) {}
  virtual Value::Type type() const override {
    return Type::INTEGER;
  }
  virtual IntegerExpression* copy() const override;
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
  int value;
};

class IdentifierExpression : public Expression {
public:
  IdentifierExpression(const std::string& identifier)
    : identifier(identifier) {}
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::string identifier;
};

class ListExpression : public Expression {
public:
  ListExpression() {}
  void push(std::unique_ptr<const Expression> expression) {
    expressions.push_back(std::move(expression));
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::vector<std::unique_ptr<const Expression>> expressions;
};

class ListValue : public Value {
public:
  ListValue() {}
  void push(std::unique_ptr<Value> value) {
    values.push_back(std::move(value));
  }
  virtual Value::Type type() const override {
    return Type::LIST;
  }
  virtual ListValue* copy() const override;
private:
  ListValue(const ListValue&);
  std::vector<std::unique_ptr<Value>> values;
};

class BinaryExpression : public Expression {
public:
  BinaryExpression
    (const Operator& operator_,
     std::unique_ptr<Expression> a,
     std::unique_ptr<Expression> b)
    : operator_(operator_), a(std::move(a)), b(std::move(b)) {}
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  Operator operator_;
  std::unique_ptr<const Expression> a;
  std::unique_ptr<const Expression> b;
};

class UnaryExpression : public Expression {
public:
  UnaryExpression
    (const Operator& operator_,
     std::unique_ptr<const Expression> a)
    : operator_(operator_), a(std::move(a)) {}
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  Operator operator_;
  std::unique_ptr<const Expression> a;
};

class UndefinedExpression : public Expression, public Value {
public:
  virtual Value::Type type() const override {
    return Type::UNDEFINED;
  }
  virtual UndefinedExpression* copy() const override;
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
};

}

#endif
