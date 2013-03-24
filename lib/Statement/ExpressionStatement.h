#ifndef HAP_EXPRESSIONSTATEMENT_H
#define HAP_EXPRESSIONSTATEMENT_H

#include "Statement.h"

namespace hap {

class ExpressionStatement : public Statement {
public:
  ExpressionStatement(std::shared_ptr<Expression>);
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::shared_ptr<Expression> expression;
};

}

#endif
