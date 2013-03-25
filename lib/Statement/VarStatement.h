#ifndef HAP_VARSTATEMENT_H
#define HAP_VARSTATEMENT_H

#include "Statement.h"

#include <string>

namespace hap {

class VarStatement : public Statement {
public:
  VarStatement(const std::string&, std::shared_ptr<Expression>);
protected:
  virtual bool equal(const Statement&) const;
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::string identifier;
  std::shared_ptr<Expression> initializer;
};

}

#endif
