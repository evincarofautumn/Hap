#ifndef HAP_TRACESTATEMENT_H
#define HAP_TRACESTATEMENT_H

#include "Statement.h"

namespace hap {

class TraceStatement : public Statement {
public:
  TraceStatement(std::shared_ptr<Expression>);
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::shared_ptr<Expression> expression;
};

}

#endif
