#ifndef HAP_FLOWSTATEMENT_H
#define HAP_FLOWSTATEMENT_H

#include "Statement.h"

namespace hap {

class FlowStatement : public Statement {
public:
  FlowStatement
    (const std::string&,
     std::unique_ptr<Expression>,
     std::unique_ptr<Statement>);
  virtual std::unique_ptr<Value> exec(Environment&) const = 0;
  virtual void write(std::ostream&) const override;
protected:
  std::string keyword;
  std::unique_ptr<Expression> expression;
  std::unique_ptr<Statement> statement;
};

#define FLOW_STATEMENT(NAME, KEYWORD) \
class NAME##Statement : public FlowStatement { \
public: \
  NAME##Statement \
    (std::unique_ptr<Expression>, \
     std::unique_ptr<Statement>); \
  virtual std::unique_ptr<Value> exec(Environment&) const override; \
};

FLOW_STATEMENT(If, "if")
FLOW_STATEMENT(When, "when")
FLOW_STATEMENT(Whenever, "whenever")
FLOW_STATEMENT(While, "while")
FLOW_STATEMENT(RepeatWhen, "repeat_when")
FLOW_STATEMENT(RepeatWhenever, "repeat_whenever")

#undef FLOW_STATEMENT

}

#endif
