#ifndef HAP_FUNSTATEMENT_H
#define HAP_FUNSTATEMENT_H

#include "Statement.h"

#include "Environment.h"

#include <string>
#include <vector>

namespace hap {

class FunStatement : public Statement {
public:
  FunStatement
    (const std::string&,
     const std::vector<std::string>&,
     std::shared_ptr<Statement>,
     const Environment&);
  virtual void exec(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::string identifier;
  std::vector<std::string> parameters;
  std::shared_ptr<Statement> body;
  mutable Environment local;
};

}

#endif