#ifndef HAP_RETSTATEMENT_H
#define HAP_RETSTATEMENT_H

#include "Statement.h"

namespace hap {

class RetStatement : public Statement {
public:
  RetStatement(std::shared_ptr<Expression>);
protected:
  virtual bool equal(const Statement&) const;
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::shared_ptr<Expression> expression;
};

}

#endif
