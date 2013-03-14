#ifndef HAP_NEXTSTATEMENT_H
#define HAP_NEXTSTATEMENT_H

#include "Statement.h"

namespace hap {

class NextStatement : public Statement {
public:
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
};

}

#endif
