#include "Expression.h"

#include <ostream>

using namespace std;

namespace hap {

Expression::~Expression() {}

ostream& operator<<(ostream& stream, const Expression& expression) {
  expression.write(stream);
  return stream;
}

void IntegerExpression::write(ostream& stream) const {
  stream << value;
}

void IdentifierExpression::write(ostream& stream) const {
  stream << identifier;
}

void ListExpression::write(ostream& stream) const {
  stream << "[ ";
  for (const auto& expression : expressions) {
    expression->write(stream);
    stream << ' ';
  }
  stream << ']';
}

void BinaryExpression::write(ostream& stream) const {
  stream << '(';
  a->write(stream);
  stream << ' ' << operator_ << ' ';
  b->write(stream);
  stream << ')';
}

void UnaryExpression::write(ostream& stream) const {
  stream << '(' << operator_;
  a->write(stream);
  stream << ')';
}

}
