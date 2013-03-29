#ifndef HAP_CONTROLSTATEMENT_H
#define HAP_CONTROLSTATEMENT_H

#include "Statement.h"

#include <string>

namespace hap {

class ControlStatement : public Statement {
public:
  enum Control {
    EXIT,
    LAST,
    NEXT,
    REDO,
  };
  ControlStatement(Control control)
    : control(control) {}
protected:
  virtual bool equal(const Statement&) const;
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const = 0;
  virtual void write(std::ostream&) const final override;
  Control control;
};

std::ostream& operator<<(std::ostream&, ControlStatement::Control);

#define CONTROL_STATEMENT(NAME, KEYWORD) \
class NAME##Statement : public ControlStatement { \
public: \
  NAME##Statement() : ControlStatement(KEYWORD) {} \
  virtual void exec \
    (Context&, std::shared_ptr<Environment>) const final override; \
};

CONTROL_STATEMENT(Exit, EXIT)
CONTROL_STATEMENT(Last, LAST)
CONTROL_STATEMENT(Next, NEXT)
CONTROL_STATEMENT(Redo, REDO)

#undef CONTROL_STATEMENT

}

#endif
