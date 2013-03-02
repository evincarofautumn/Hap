#ifndef HAP_EXPRESSION_H
#define HAP_EXPRESSION_H

#include <string>

namespace hap {

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

}

#endif
