#ifndef HAP_STATEMENT_H
#define HAP_STATEMENT_H

#include "Expression.h"

#include <memory>
#include <string>
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
  void push(std::unique_ptr<Statement> statement) {
    statements.push_back(std::move(statement));
  }
private:
  std::vector< std::unique_ptr<Statement> > statements;
};

class VarStatement : public Statement {
public:
  VarStatement(const std::string& identifier,
               std::unique_ptr<Expression> initializer)
    : identifier(identifier), initializer(std::move(initializer)) {}
private:
  std::string identifier;
  std::unique_ptr<Expression> initializer;
};

class FlowStatement : public Statement {
public:
  FlowStatement(std::unique_ptr<Expression> expression,
                std::unique_ptr<Statement> statement)
    : expression(std::move(expression)),
      statement(std::move(statement)) {}
private:
  std::unique_ptr<Expression> expression;
  std::unique_ptr<Statement> statement;
};

#define FLOW_STATEMENT(NAME) \
struct NAME##Statement : FlowStatement { \
  NAME##Statement(std::unique_ptr<Expression> expression, \
                  std::unique_ptr<Statement> statement) \
    : FlowStatement(std::move(expression), std::move(statement)) {} \
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
