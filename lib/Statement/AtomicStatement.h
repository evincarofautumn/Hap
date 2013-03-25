#ifndef HAP_ATOMICSTATEMENT_H
#define HAP_ATOMICSTATEMENT_H

#include "Statement.h"

namespace hap {

class AtomicStatement : public Statement {
public:
  AtomicStatement(std::shared_ptr<Statement>);
protected:
  virtual bool equal(const Statement&) const;
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::shared_ptr<Statement> statement;
};

}

#endif
