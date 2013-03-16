#ifndef HAP_LISTEXPRESSION_H
#define HAP_LISTEXPRESSION_H

#include "Expression.h"

#include <vector>

namespace hap {

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

}

#endif
