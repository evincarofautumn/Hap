#ifndef HAP_LASTSTATEMENT_H
#define HAP_LASTSTATEMENT_H

#include "Statement.h"

namespace hap {

class LastStatement : public Statement {
public:
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
};

}

#endif
