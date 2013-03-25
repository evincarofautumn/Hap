#ifndef HAP_LISTEXPRESSION_H
#define HAP_LISTEXPRESSION_H

#include "Expression.h"

#include <vector>

namespace hap {

class ListExpression : public Expression {
public:
  ListExpression() {}
  void push(std::shared_ptr<const Expression> expression) {
    expressions.push_back(expression);
  }
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::vector<std::shared_ptr<const Expression>> expressions;
};

}

#endif
