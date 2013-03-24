#ifndef HAP_BINARYEXPRESSION_H
#define HAP_BINARYEXPRESSION_H

#include "Expression.h"

#include "Operator.h"

namespace hap {

class BinaryExpression : public Expression {
public:
  BinaryExpression
    (const Operator& operator_,
     std::shared_ptr<Expression> left,
     std::shared_ptr<Expression> right)
    : operator_(operator_),
      left(left),
      right(right) {}
  virtual std::shared_ptr<Value> eval
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  Operator operator_;
  std::shared_ptr<const Expression> left;
  std::shared_ptr<const Expression> right;
};

}

#endif
