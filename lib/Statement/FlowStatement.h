#ifndef HAP_FLOWSTATEMENT_H
#define HAP_FLOWSTATEMENT_H

#include "Statement.h"

#include <string>

namespace hap {

class FlowStatement : public Statement {
public:
  FlowStatement
    (const std::string&,
     std::shared_ptr<Expression>,
     std::shared_ptr<Statement>);
protected:
  virtual bool equal(const Statement&) const;
  virtual void exec
    (Context&, std::shared_ptr<Environment>) const = 0;
  virtual void write(std::ostream&) const final override;
  std::string keyword;
  std::shared_ptr<Expression> expression;
  std::shared_ptr<Statement> statement;
};

#define FLOW_STATEMENT(NAME, KEYWORD) \
class NAME##Statement : public FlowStatement { \
public: \
  NAME##Statement \
    (std::shared_ptr<Expression>, \
     std::shared_ptr<Statement>); \
  virtual void exec \
    (Context&, std::shared_ptr<Environment>) const final override; \
};

FLOW_STATEMENT(If, "if")
FLOW_STATEMENT(When, "when")
FLOW_STATEMENT(Whenever, "whenever")
FLOW_STATEMENT(While, "while")

#undef FLOW_STATEMENT

}

#endif
