#ifndef HAP_SUBSCRIPTEXPRESSION_H
#define HAP_SUBSCRIPTEXPRESSION_H

#include "Expression.h"

#include "binary.h"

#include <vector>

namespace hap {

class SubscriptExpression : public Expression {
public:
  friend Operator::Binary binary::assign;
  SubscriptExpression(Expression*, Expression*);
  SubscriptExpression
    (std::shared_ptr<Expression>,
     std::shared_ptr<Expression>);
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::shared_ptr<Expression> expression;
  std::shared_ptr<Expression> subscript;
};

}

#endif
