#include "Statement.h"
#include "Environment.h"
#include "Expression.h"

#include <ostream>

using namespace std;

namespace hap {

Statement::~Statement() {}

ostream& operator<<(ostream& stream, const Statement& statement) {
  statement.write(stream);
  return stream;
}

void BlockStatement::exec(Environment& environment) const {
  for (const auto& statement : statements)
    statement->exec(environment);
}

void BlockStatement::write(ostream& stream) const {
  stream << "{\n";
  for (const auto& statement : statements)
    statement->write(stream);
  stream << "}\n";
}

VarStatement::VarStatement
  (const std::string& identifier,
   std::unique_ptr<Expression> initializer)
  : identifier(identifier),
    initializer(std::move(initializer)) {}

void VarStatement::exec(Environment& environment) const {
  auto value = initializer
    ? initializer->eval(environment)
    : unique_ptr<Value>(new UndefinedExpression());
  environment.define(identifier, move(value));
}

void VarStatement::write(ostream& stream) const {
  stream << "var " << identifier;
  if (initializer) {
    stream << " = ";
    initializer->write(stream);
  }
  stream << ";\n";
}

FunStatement::FunStatement
  (const std::string& identifier,
   const std::vector<std::string>& parameters,
   std::shared_ptr<Statement> body,
   const Environment& environment)
  : identifier(identifier),
    parameters(parameters),
    body(body),
    local(environment) {}

void FunStatement::exec(Environment& environment) const {
  unique_ptr<Value> value
    (new FunExpression(identifier, parameters, body, local));
  environment.define(identifier, move(value));
}

void FunStatement::write(ostream& stream) const {
  stream << "fun " << identifier << "(";
  for (const auto& parameter : parameters)
    stream << parameter << ", ";
  stream << ") ";
  body->write(stream);
}

RetStatement::RetStatement(std::unique_ptr<Expression> expression)
  : expression(std::move(expression)) {}

void RetStatement::exec(Environment& environment) const {
  throw runtime_error("unimplemented ret");
}

void RetStatement::write(ostream& stream) const {
  stream << "ret ";
  expression->write(stream);
  stream << ";\n";
}

FlowStatement::FlowStatement
  (const std::string& keyword,
   std::unique_ptr<Expression> expression,
   std::unique_ptr<Statement> statement)
  : keyword(keyword),
    expression(std::move(expression)),
    statement(std::move(statement)) {}

void FlowStatement::exec(Environment& environment) const {
  throw runtime_error("unimplemented flow");
}

void FlowStatement::write(ostream& stream) const {
  stream << keyword << ' ';
  expression->write(stream);
  stream << '\n';
  statement->write(stream);
}

#define FLOW_STATEMENT(NAME, KEYWORD) \
  NAME##Statement::NAME##Statement \
    (std::unique_ptr<Expression> expression, \
     std::unique_ptr<Statement> statement) \
    : FlowStatement(KEYWORD, std::move(expression), std::move(statement)) {}

FLOW_STATEMENT(If, "if")
FLOW_STATEMENT(When, "when")
FLOW_STATEMENT(Whenever, "whenever")
FLOW_STATEMENT(While, "while")
FLOW_STATEMENT(RepeatWhen, "repeat_when")
FLOW_STATEMENT(RepeatWhenever, "repeat_whenever")

#undef FLOW_STATEMENT

}
