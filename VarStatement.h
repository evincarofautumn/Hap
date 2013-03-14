#ifndef HAP_VARSTATEMENT_H
#define HAP_VARSTATEMENT_H

#include "Statement.h"

namespace hap {

class VarStatement : public Statement {
public:
  VarStatement(const std::string&, std::unique_ptr<Expression>);
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::string identifier;
  std::unique_ptr<Expression> initializer;
};

}

#endif
