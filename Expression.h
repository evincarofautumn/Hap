#ifndef HAP_EXPRESSION_H
#define HAP_EXPRESSION_H

#include <string>
#include <vector>

namespace hap {

class Expression {
public:
  virtual ~Expression();
};

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
  virtual ~ListExpression() {
    for (std::vector<Expression*>::iterator i = expressions.begin();
         i != expressions.end();
         ++i) {
      delete *i;
    }
  }
  void push(Expression* const expression) {
    expressions.push_back(expression);
  }
private:
  std::vector<Expression*> expressions;
};

}

#endif
