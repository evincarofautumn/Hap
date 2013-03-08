#include "Expression.h"
#include "Environment.h"
#include "unique_cast.h"

#include <ostream>
#include <sstream>

using namespace std;

namespace hap {

void Value::assert_type(Type expected) const {
  auto actual(type());
  if (actual == expected)
    return;
  ostringstream message;
  message << "expected " << expected << " but got " << actual;
  throw runtime_error(message.str());
}

ostream& operator<<(ostream& stream, const Value::Type& type) {
  switch (type) {
  case Value::UNDEFINED: return stream << "undefined";
  case Value::BOOLEAN: return stream << "boolean";
  case Value::FUNCTION: return stream << "function";
  case Value::INTEGER: return stream << "integer";
  case Value::LIST: return stream << "list";
  case Value::MAP: return stream << "map";
  case Value::STRING: return stream << "string";
  }
}

Expression::~Expression() {}

ostream& operator<<(ostream& stream, const Expression& expression) {
  expression.write(stream);
  return stream;
}

Value::~Value() {}

unique_ptr<Value> BooleanExpression::eval(Environment&) const {
  return unique_ptr<Value>(new BooleanExpression(*this));
}

void BooleanExpression::write(ostream& stream) const {
  stream << (value ? "true" : "false");
}

CallExpression::CallExpression
  (std::string&& identifier,
   std::vector<std::unique_ptr<Expression>>&& expressions)
  : identifier(move(identifier)),
    expressions(move(expressions)) {}

std::unique_ptr<Value> CallExpression::eval(Environment&) const {
  throw runtime_error("unimplemented call");
}

void CallExpression::write(std::ostream& stream) const {
  stream << identifier << "(";
  for (const auto& expression : expressions) {
    expression->write(stream);
    stream << ", ";
  }
  stream << ")";
}

FunExpression::FunExpression
  (const std::string& identifier,
   const std::vector<std::string>& parameters,
   std::shared_ptr<Statement> body,
   const Environment& environment)
  : identifier(identifier),
    parameters(parameters),
    body(body),
    environment(environment) {}

unique_ptr<Value> FunExpression::eval(Environment& environment) const {
  throw runtime_error("unimplemented fun");
}

void FunExpression::write(ostream& stream) const {
  stream << "\\" << identifier << "(";
  for (const auto& parameter : parameters)
    stream << parameter << ", ";
  stream << ") ";
  body->write(stream);
}

unique_ptr<Value> IntegerExpression::eval(Environment&) const {
  return unique_ptr<Value>(new IntegerExpression(*this));
}

void IntegerExpression::write(ostream& stream) const {
  stream << value;
}

unique_ptr<Value> StringExpression::eval(Environment&) const {
  return unique_ptr<Value>(new StringExpression(*this));
}

void StringExpression::write(ostream& stream) const {
  stream << '"' << value << '"';
}

unique_ptr<Value> IdentifierExpression::eval(Environment& environment) const {
  return unique_ptr<Value>(environment[identifier].copy());
}

void IdentifierExpression::write(ostream& stream) const {
  stream << identifier;
}

unique_ptr<Value> ListExpression::eval(Environment& environment) const {
  unique_ptr<ListValue> list(new ListValue());
  for (const auto& expression : expressions)
    list->push(expression->eval(environment));
  return static_unique_cast<Value>(move(list));
}

void ListExpression::write(ostream& stream) const {
  stream << "[ ";
  for (const auto& expression : expressions) {
    expression->write(stream);
    stream << ", ";
  }
  stream << ']';
}

ListValue::ListValue(const ListValue& other) {
  for (const auto& value : other.values)
    values.push_back(unique_ptr<Value>(value->copy()));
}

unique_ptr<Value> MapExpression::eval(Environment& environment) const {
  unique_ptr<MapValue> map(new MapValue());
  for (const auto& pair : pairs) {
    auto key(pair.first->eval(environment));
    auto value(pair.second->eval(environment));
    map->insert(move(key), move(value));
  }
  return static_unique_cast<Value>(move(map));
}

void MapExpression::write(ostream& stream) const {
  stream << "{ ";
  for (const auto& pair : pairs) {
    pair.first->write(stream);
    stream << ": ";
    pair.second->write(stream);
    stream << ", ";
  }
  stream << '}';
}

MapValue::MapValue(const MapValue& other) {
  for (const auto& pair : other.pairs)
    pairs.insert(make_pair(unique_ptr<Value>(pair.first->copy()),
                           unique_ptr<Value>(pair.second->copy())));
}

unique_ptr<Value> BinaryExpression::eval(Environment& environment) const {
  if (operator_.binary)
    return operator_.binary(environment, left, right);
  ostringstream message;
  message << "unimplemented binary operator " << operator_;
  throw runtime_error(message.str());
}

void BinaryExpression::write(ostream& stream) const {
  stream << '(';
  left->write(stream);
  stream << ' ' << operator_ << ' ';
  right->write(stream);
  stream << ')';
}

unique_ptr<Value> UnaryExpression::eval(Environment& environment) const {
  if (operator_.unary)
    return operator_.unary(environment, expression);
  ostringstream message;
  message << "unimplemented unary operator " << operator_;
  throw runtime_error(message.str());
}

void UnaryExpression::write(ostream& stream) const {
  stream << '(' << operator_ << ' ';
  expression->write(stream);
  stream << ')';
}

unique_ptr<Value> UndefinedExpression::eval(Environment& environment) const {
  return unique_ptr<Value>(new UndefinedExpression(*this));
}

void UndefinedExpression::write(ostream& stream) const {
  stream << "undef";
}

}
