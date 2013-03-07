#ifndef HAP_EXPRESSION_H
#define HAP_EXPRESSION_H

#include "Operator.h"

#include <iosfwd>
#include <map>
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
    BOOLEAN,
    INTEGER,
    LIST,
    MAP,
    STRING,
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

class BooleanExpression : public Expression, public Value {
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

class IntegerExpression : public Expression, public Value {
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

class StringExpression : public Expression, public Value {
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
  virtual ListValue* copy() const override {
    return new ListValue(*this);
  }
private:
  ListValue(const ListValue&);
  std::vector<std::unique_ptr<Value>> values;
};

class MapExpression : public Expression {
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
private:
  MapValue(const MapValue&);
  std::map<std::unique_ptr<Value>, std::unique_ptr<Value>> pairs;
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
