#ifndef HAP_LASTSTATEMENT_H
#define HAP_LASTSTATEMENT_H

#include "Statement.h"

namespace hap {

class LastStatement : public Statement {
public:
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
};

}

#endif
