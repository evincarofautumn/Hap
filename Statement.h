#ifndef HAP_STATEMENT_H
#define HAP_STATEMENT_H

#include "Expression.h"

#include <string>
#include <vector>

namespace hap {

class Expression;

class Statement {
public:
  virtual ~Statement();
protected:
  Statement();
private:
  Statement(const Statement&);
  Statement& operator=(const Statement&);
};

class BlockStatement : public Statement {
public:
  BlockStatement() {}
  virtual ~BlockStatement() {
    for (std::vector<Statement*>::iterator i = statements.begin();
         i != statements.end();
         ++i) {
      delete *i;
    }
  }
  void push(Statement* const statement) {
    statements.push_back(statement);
  }
private:
  std::vector<Statement*> statements;
};

class VarStatement : public Statement {
public:
  VarStatement(const std::string& identifier, Expression* const initializer)
    : identifier(identifier), initializer(initializer) {}
  virtual ~VarStatement() {
    delete initializer;
  }
private:
  std::string identifier;
  Expression* initializer;
};

class FlowStatement : public Statement {
public:
  FlowStatement(Expression* const expression, Statement* const statement)
    : expression(expression), statement(statement) {}
  virtual ~FlowStatement() {
    delete expression;
    delete statement;
  }
private:
  Expression* expression;
  Statement* statement;
};

#define FLOW_STATEMENT(NAME) \
struct NAME##Statement : FlowStatement { \
  NAME##Statement \
    (Expression* const expression, Statement* const statement) \
    : FlowStatement(expression, statement) {} \
};

FLOW_STATEMENT(If)
FLOW_STATEMENT(When)
FLOW_STATEMENT(Whenever)
FLOW_STATEMENT(While)
FLOW_STATEMENT(RepeatWhen)
FLOW_STATEMENT(RepeatWhenever)

#undef FLOW_STATEMENT

}

#endif
