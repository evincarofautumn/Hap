#ifndef HAP_DELSTATEMENT_H
#define HAP_DELSTATEMENT_H

#include "Statement.h"

#include <string>

namespace hap {

class DelStatement : public Statement {
public:
  DelStatement(const std::string&);
protected:
  virtual bool equal(const Statement&) const;
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::string identifier;
};

}

#endif
