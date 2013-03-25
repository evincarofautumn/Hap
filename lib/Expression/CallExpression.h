#ifndef HAP_CALLEXPRESSION_H
#define HAP_CALLEXPRESSION_H

#include "Expression.h"

#include <vector>

namespace hap {

class CallExpression : public Expression {
public:
  CallExpression(Expression*, std::initializer_list<Expression*>);
  CallExpression
    (std::shared_ptr<Expression>,
     std::vector<std::shared_ptr<Expression>>&&);
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::shared_ptr<Expression> function;
  std::vector<std::shared_ptr<Expression>> expressions;
};

}

#endif
