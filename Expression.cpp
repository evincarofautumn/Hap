#include "Expression.h"
#include "Environment.h"
#include "unique_cast.h"

#include <ostream>
#include <sstream>

using namespace std;

namespace hap {

Expression::~Expression() {}

ostream& operator<<(ostream& stream, const Expression& expression) {
  expression.write(stream);
  return stream;
}

Value::~Value() {}

IntegerExpression* IntegerExpression::copy() const {
  return new IntegerExpression(*this);
}

unique_ptr<Value> IntegerExpression::eval(Environment&) const {
  return unique_ptr<Value>(new IntegerExpression(*this));
}

void IntegerExpression::write(ostream& stream) const {
  stream << value;
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
    stream << ' ';
  }
  stream << ']';
}

ListValue::ListValue(const ListValue& other) {
  for (const auto& value : other.values)
    values.push_back(unique_ptr<Value>(value->copy()));
}

ListValue* ListValue::copy() const {
  return new ListValue(*this);
}

unique_ptr<Value> BinaryExpression::eval(Environment& environment) const {
  ostringstream message;
  message << "unimplemented evaluation of binary " << operator_;
  throw runtime_error(message.str());
}

void BinaryExpression::write(ostream& stream) const {
  stream << '(';
  a->write(stream);
  stream << ' ' << operator_ << ' ';
  b->write(stream);
  stream << ')';
}

unique_ptr<Value> UnaryExpression::eval(Environment& environment) const {
  ostringstream message;
  message << "unimplemented evaluation of unary " << operator_;
  throw runtime_error(message.str());
}

void UnaryExpression::write(ostream& stream) const {
  stream << '(' << operator_ << ' ';
  a->write(stream);
  stream << ')';
}

UndefinedExpression* UndefinedExpression::copy() const {
  return new UndefinedExpression();
}

unique_ptr<Value> UndefinedExpression::eval(Environment& environment) const {
  return unique_ptr<Value>(new UndefinedExpression(*this));
}

void UndefinedExpression::write(ostream& stream) const {
  stream << "undef";
}

}
