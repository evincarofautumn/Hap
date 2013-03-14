#ifndef HAP_EXPRESSION_H
#define HAP_EXPRESSION_H

#include "Environment.h"
#include "Operator.h"
#include "Statement.h"

#include <iosfwd>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace hap {

class Value;

class Expression {
public:
  virtual ~Expression();
  virtual std::unique_ptr<Value> eval(Environment&) const = 0;
  virtual void write(std::ostream&) const = 0;
};

class Value : virtual public Expression {
public:
  enum Type {
    UNDEFINED,
    BOOLEAN,
    FUNCTION,
    INTEGER,
    LIST,
    MAP,
    STRING,
  };
  void assert_type(Type) const;
  virtual ~Value();
  virtual std::unique_ptr<Value> eval(Environment&) const {
    return std::unique_ptr<Value>(copy());
  }
  virtual Value* copy() const = 0;
  virtual Type type() const = 0;
};

std::ostream& operator<<(std::ostream&, const Value::Type&);
std::ostream& operator<<(std::ostream&, const Expression&);

class BooleanExpression : virtual public Expression, public Value {
public:
  BooleanExpression(bool value)
    : value(value) {}
  virtual Value::Type type() const override {
    return Type::BOOLEAN;
  }
  virtual BooleanExpression* copy() const override {
    return new BooleanExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
  bool value;
};

class CallExpression : virtual public Expression {
public:
  CallExpression
    (std::unique_ptr<Expression>,
     std::vector<std::unique_ptr<Expression>>&&);
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::unique_ptr<Expression> function;
  std::vector<std::unique_ptr<Expression>> expressions;
};

class FunExpression : virtual public Expression, public Value {
public:
  FunExpression
    (const std::string&,
     const std::vector<std::string>&,
     std::shared_ptr<Statement>,
     Environment&);
  virtual Value::Type type() const override {
    return Type::FUNCTION;
  }
  virtual FunExpression* copy() const override {
    return new FunExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
  std::unique_ptr<Value> call
    (const std::vector<std::unique_ptr<Expression>>&) const;
private:
  FunExpression(const FunExpression&) = default;
  std::string identifier;
  std::vector<std::string> parameters;
  std::shared_ptr<Statement> body;
  mutable Environment environment;
};

class IntegerExpression : virtual public Expression, public Value {
public:
  IntegerExpression(int value)
    : value(value) {}
  virtual Value::Type type() const override {
    return Type::INTEGER;
  }
  virtual IntegerExpression* copy() const override {
    return new IntegerExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
  int value;
};

class StringExpression : virtual public Expression, public Value {
public:
  StringExpression(const std::string& value)
    : value(value) {}
  virtual Value::Type type() const override {
    return Type::STRING;
  }
  virtual StringExpression* copy() const override {
    return new StringExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
  std::string value;
};

class IdentifierExpression : virtual public Expression {
public:
  IdentifierExpression(const std::string& identifier)
    : identifier(identifier) {}
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::string identifier;
};

class ListExpression : virtual public Expression {
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
  virtual ListValue* copy() const override {
    return new ListValue(*this);
  }
  virtual void write(std::ostream&) const override;
private:
  ListValue(const ListValue&);
  std::vector<std::unique_ptr<Value>> values;
};

class MapExpression : virtual public Expression {
public:
  MapExpression() {}
  void insert(std::unique_ptr<const Expression> key,
              std::unique_ptr<const Expression> value) {
    pairs.insert(std::make_pair(std::move(key), std::move(value)));
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::map<std::unique_ptr<const Expression>,
           std::unique_ptr<const Expression>> pairs;
};

class MapValue : public Value {
public:
  MapValue() {}
  void insert(std::unique_ptr<Value> key, std::unique_ptr<Value> value) {
    pairs.insert(std::make_pair(std::move(key), std::move(value)));
  }
  virtual Value::Type type() const override {
    return Type::MAP;
  }
  virtual MapValue* copy() const override {
    return new MapValue(*this);
  }
  virtual void write(std::ostream&) const override;
private:
  MapValue(const MapValue&);
  std::map<std::unique_ptr<Value>, std::unique_ptr<Value>> pairs;
};

class BinaryExpression : virtual public Expression {
public:
  BinaryExpression
    (const Operator& operator_,
     std::unique_ptr<Expression> left,
     std::unique_ptr<Expression> right)
    : operator_(operator_),
      left(std::move(left)),
      right(std::move(right)) {}
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  Operator operator_;
  std::unique_ptr<const Expression> left;
  std::unique_ptr<const Expression> right;
};

class UnaryExpression : virtual public Expression {
public:
  UnaryExpression
    (const Operator& operator_,
     std::unique_ptr<const Expression> expression)
    : operator_(operator_),
      expression(std::move(expression)) {}
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  Operator operator_;
  std::unique_ptr<const Expression> expression;
};

class UndefinedExpression : virtual public Expression, public Value {
public:
  UndefinedExpression() = default;
  virtual Value::Type type() const override {
    return Type::UNDEFINED;
  }
  virtual UndefinedExpression* copy() const override {
    return new UndefinedExpression(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  UndefinedExpression(const UndefinedExpression&) = default;
};

}

#endif
