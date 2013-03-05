#ifndef HAP_STATEMENT_H
#define HAP_STATEMENT_H

#include "Expression.h"

#include <memory>
#include <string>
#include <vector>

namespace hap {

class Environment;
class Expression;

class Statement {
public:
  virtual ~Statement();
  virtual void exec(Environment&) const = 0;
  virtual void write(std::ostream&) const = 0;
protected:
  Statement() {}
private:
  Statement(const Statement&);
  Statement& operator=(const Statement&);
};

std::ostream& operator<<(std::ostream&, const Statement&);

class BlockStatement : public Statement {
public:
  BlockStatement() {}
  void push(std::unique_ptr<Statement> statement) {
    statements.push_back(std::move(statement));
  }
  virtual void exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::vector<std::unique_ptr<Statement>> statements;
};

class VarStatement : public Statement {
public:
  VarStatement
    (const std::string& identifier,
     std::unique_ptr<Expression> initializer)
    : identifier(identifier), initializer(std::move(initializer)) {}
  virtual void exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::string identifier;
  std::unique_ptr<Expression> initializer;
};

class FlowStatement : public Statement {
public:
  FlowStatement
    (const std::string& keyword,
     std::unique_ptr<Expression> expression,
     std::unique_ptr<Statement> statement)
    : keyword(keyword),
      expression(std::move(expression)),
      statement(std::move(statement)) {}
  virtual void exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::string keyword;
  std::unique_ptr<Expression> expression;
  std::unique_ptr<Statement> statement;
};

#define FLOW_STATEMENT(NAME, KEYWORD) \
struct NAME##Statement : FlowStatement { \
  NAME##Statement \
    (std::unique_ptr<Expression> expression, \
     std::unique_ptr<Statement> statement) \
    : FlowStatement(KEYWORD, std::move(expression), std::move(statement)) {} \
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
