#ifndef HAP_EXPRESSIONSTATEMENT_H
#define HAP_EXPRESSIONSTATEMENT_H

#include "Statement.h"

namespace hap {

class ExpressionStatement : public Statement {
public:
  ExpressionStatement(std::unique_ptr<Expression>);
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::unique_ptr<Expression> expression;
};

}

#endif
