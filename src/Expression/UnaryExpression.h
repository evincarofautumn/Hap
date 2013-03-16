#ifndef HAP_UNARYEXPRESSION_H
#define HAP_UNARYEXPRESSION_H

#include "Expression.h"

#include "Operator.h"

namespace hap {

class UnaryExpression : public Expression {
public:
  UnaryExpression
    (const Operator& operator_,
     std::unique_ptr<const Expression> expression)
    : operator_(operator_),
      expression(std::move(expression)) {}
  virtual std::unique_ptr<Value> eval(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  Operator operator_;
  std::unique_ptr<const Expression> expression;
};

}

#endif
