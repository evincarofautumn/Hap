#ifndef HAP_UNARYEXPRESSION_H
#define HAP_UNARYEXPRESSION_H

#include "Expression.h"

#include "Operator.h"

namespace hap {

class UnaryExpression : public Expression {
public:
  UnaryExpression
    (const Operator& operator_,
     std::shared_ptr<const Expression> expression)
    : operator_(operator_),
      expression(expression) {}
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
protected:
  virtual bool equal(const Expression&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  Operator operator_;
  std::shared_ptr<const Expression> expression;
};

}

#endif
