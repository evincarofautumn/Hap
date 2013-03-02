#ifndef HAP_STATEMENT_H
#define HAP_STATEMENT_H

#include "Expression.h"

#include <string>
#include <tr1/memory>
#include <vector>

namespace hap {

class Expression;

class Statement {
protected:
  Statement() {}
private:
  Statement(const Statement&);
  Statement& operator=(const Statement&);
};

class BlockStatement : public Statement {
public:
  BlockStatement() {}
  void push(std::tr1::shared_ptr<Statement> const statement) {
    statements.push_back(statement);
  }
private:
  std::vector< std::tr1::shared_ptr<Statement> > statements;
};

class VarStatement : public Statement {
public:
  VarStatement(const std::string& identifier,
               std::tr1::shared_ptr<Expression> const initializer)
    : identifier(identifier), initializer(initializer) {}
private:
  std::string identifier;
  std::tr1::shared_ptr<Expression> initializer;
};

class FlowStatement : public Statement {
public:
  FlowStatement(std::tr1::shared_ptr<Expression> const expression,
                std::tr1::shared_ptr<Statement> const statement)
    : expression(expression), statement(statement) {}
private:
  std::tr1::shared_ptr<Expression> expression;
  std::tr1::shared_ptr<Statement> statement;
};

#define FLOW_STATEMENT(NAME) \
struct NAME##Statement : FlowStatement { \
  NAME##Statement(std::tr1::shared_ptr<Expression> const expression, \
                  std::tr1::shared_ptr<Statement> const statement)   \
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
