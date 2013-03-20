#ifndef HAP_TRACESTATEMENT_H
#define HAP_TRACESTATEMENT_H

#include "Statement.h"

namespace hap {

class TraceStatement : public Statement {
public:
  TraceStatement(std::unique_ptr<Expression>);
  virtual void exec(std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::unique_ptr<Expression> expression;
};

}

#endif
