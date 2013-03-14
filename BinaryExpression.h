#ifndef HAP_BINARYEXPRESSION_H
#define HAP_BINARYEXPRESSION_H

#include "Expression.h"

namespace hap {

class BinaryExpression : public Expression {
public:
  BinaryExpression
    (const Operator& operator_,
     std::unique_ptr<Expression> left,
     std::unique_ptr<Expression> right)
    : operator_(operator_),
      left(std::move(left)),
      right(std::move(right)) {}
  virtual std::unique_ptr<Value> eval(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  Operator operator_;
  std::unique_ptr<const Expression> left;
  std::unique_ptr<const Expression> right;
};

}

#endif
