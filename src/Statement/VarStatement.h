#ifndef HAP_VARSTATEMENT_H
#define HAP_VARSTATEMENT_H

#include "Statement.h"

#include <string>

namespace hap {

class VarStatement : public Statement {
public:
  VarStatement(const std::string&, std::unique_ptr<Expression>);
  virtual void exec(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::string identifier;
  std::unique_ptr<Expression> initializer;
};

}

#endif
