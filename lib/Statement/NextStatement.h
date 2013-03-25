#ifndef HAP_NEXTSTATEMENT_H
#define HAP_NEXTSTATEMENT_H

#include "Statement.h"

namespace hap {

class NextStatement : public Statement {
protected:
  virtual bool equal(const Statement&) const;
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
};

}

#endif
