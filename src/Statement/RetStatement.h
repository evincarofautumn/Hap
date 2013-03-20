#ifndef HAP_RETSTATEMENT_H
#define HAP_RETSTATEMENT_H

#include "Statement.h"

namespace hap {

class RetStatement : public Statement {
public:
  RetStatement(std::unique_ptr<Expression>);
  virtual void exec(std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::unique_ptr<Expression> expression;
};

}

#endif
