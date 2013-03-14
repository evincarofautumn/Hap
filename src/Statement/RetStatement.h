#ifndef HAP_RETSTATEMENT_H
#define HAP_RETSTATEMENT_H

#include "Statement.h"

namespace hap {

class RetStatement : public Statement {
public:
  RetStatement(std::unique_ptr<Expression>);
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::unique_ptr<Expression> expression;
};

}

#endif
