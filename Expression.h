#ifndef HAP_EXPRESSION_H
#define HAP_EXPRESSION_H

#include <string>
#include <tr1/memory>
#include <vector>

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

class ListExpression : public Expression {
public:
  ListExpression() {}
  void push(std::tr1::shared_ptr<Expression> const expression) {
    expressions.push_back(expression);
  }
private:
  std::vector< std::tr1::shared_ptr<Expression> > expressions;
};

}

#endif
