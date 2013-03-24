#ifndef HAP_RETSTATEMENT_H
#define HAP_RETSTATEMENT_H

#include "Statement.h"

namespace hap {

class RetStatement : public Statement {
public:
  RetStatement(std::shared_ptr<Expression>);
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::shared_ptr<Expression> expression;
};

}

#endif
