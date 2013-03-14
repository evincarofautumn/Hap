#ifndef HAP_FUNSTATEMENT_H
#define HAP_FUNSTATEMENT_H

#include "Statement.h"

namespace hap {

class FunStatement : public Statement {
public:
  FunStatement
    (const std::string&,
     const std::vector<std::string>&,
     std::shared_ptr<Statement>,
     const Environment&);
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::string identifier;
  std::vector<std::string> parameters;
  std::shared_ptr<Statement> body;
  mutable Environment local;
};

}

#endif
