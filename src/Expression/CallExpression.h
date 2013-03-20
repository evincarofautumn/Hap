#ifndef HAP_CALLEXPRESSION_H
#define HAP_CALLEXPRESSION_H

#include "Expression.h"

#include <vector>

namespace hap {

class CallExpression : public Expression {
public:
  CallExpression
    (std::unique_ptr<Expression>,
     std::vector<std::unique_ptr<Expression>>&&);
  virtual std::shared_ptr<Value> eval(std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::unique_ptr<Expression> function;
  std::vector<std::unique_ptr<Expression>> expressions;
};

}

#endif
