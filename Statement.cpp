#include "Statement.h"
#include "Environment.h"
#include "Expression.h"
#include "unique_cast.h"

#include <ostream>

using namespace std;

namespace hap {

Statement::~Statement() {}

ostream& operator<<(ostream& stream, const Statement& statement) {
  statement.write(stream);
  return stream;
}

unique_ptr<Value> BlockStatement::exec(Environment& environment) const {
  unique_ptr<Value> result;
  for (const auto& statement : statements)
    result = statement->exec(environment);
  return move(result);
}

void BlockStatement::write(ostream& stream) const {
  stream << "{\n";
  for (const auto& statement : statements)
    statement->write(stream);
  stream << "}\n";
}

VarStatement::VarStatement
  (const string& identifier,
   unique_ptr<Expression> initializer)
  : identifier(identifier),
    initializer(move(initializer)) {}

unique_ptr<Value> VarStatement::exec(Environment& environment) const {
  auto value = initializer
    ? initializer->eval(environment)
    : unique_ptr<Value>(new UndefinedExpression());
  environment.define(identifier, move(value));
  return unique_ptr<Value>(new UndefinedExpression());
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
  (const string& identifier,
   const vector<string>& parameters,
   shared_ptr<Statement> body,
   const Environment& environment)
  : identifier(identifier),
    parameters(parameters),
    body(body),
    local(environment) {}

unique_ptr<Value> FunStatement::exec(Environment& environment) const {
  unique_ptr<Value> value
    (new FunExpression(identifier, parameters, body, local));
  environment.define(identifier, move(value));
  return unique_ptr<Value>(new UndefinedExpression());
}

void FunStatement::write(ostream& stream) const {
  stream << "fun " << identifier << "(";
  for (const auto& parameter : parameters)
    stream << parameter << ", ";
  stream << ") ";
  body->write(stream);
}

unique_ptr<Value> LastStatement::exec(Environment&) const {
  throw runtime_error("unimplemented last");
}

void LastStatement::write(ostream& stream) const {
  stream << "last;\n";
}

unique_ptr<Value> NextStatement::exec(Environment&) const {
  throw runtime_error("unimplemented next");
}

void NextStatement::write(ostream& stream) const {
  stream << "next;\n";
}

RetStatement::RetStatement(unique_ptr<Expression> expression)
  : expression(move(expression)) {}

unique_ptr<Value> RetStatement::exec(Environment& environment) const {
  throw runtime_error("unimplemented ret");
}

void RetStatement::write(ostream& stream) const {
  stream << "ret ";
  expression->write(stream);
  stream << ";\n";
}

ExpressionStatement::ExpressionStatement
  (unique_ptr<Expression> expression)
  : expression(move(expression)) {}

unique_ptr<Value> ExpressionStatement::exec(Environment& environment) const {
  return expression->eval(environment);
}

void ExpressionStatement::write(ostream& stream) const {
  expression->write(stream);
  stream << ";\n";
}

FlowStatement::FlowStatement
  (const string& keyword,
   unique_ptr<Expression> expression,
   unique_ptr<Statement> statement)
  : keyword(keyword),
    expression(move(expression)),
    statement(move(statement)) {}

void FlowStatement::write(ostream& stream) const {
  stream << keyword << ' ';
  expression->write(stream);
  stream << '\n';
  statement->write(stream);
}

#define FLOW_STATEMENT(NAME, KEYWORD) \
  NAME##Statement::NAME##Statement \
    (unique_ptr<Expression> expression, \
     unique_ptr<Statement> statement) \
    : FlowStatement(KEYWORD, move(expression), move(statement)) {}

FLOW_STATEMENT(If, "if")
FLOW_STATEMENT(When, "when")
FLOW_STATEMENT(Whenever, "whenever")
FLOW_STATEMENT(While, "while")
FLOW_STATEMENT(RepeatWhen, "repeat_when")
FLOW_STATEMENT(RepeatWhenever, "repeat_whenever")

#undef FLOW_STATEMENT

unique_ptr<Value> IfStatement::exec(Environment& environment) const {
  auto value(expression->eval(environment));
  value->assert_type(Value::BOOLEAN);
  auto condition(static_unique_cast<BooleanExpression>(move(value)));
  if (condition->value)
    statement->exec(environment);
  return unique_ptr<Value>(new UndefinedExpression());
}

unique_ptr<Value> WhenStatement::exec(Environment&) const {
  throw runtime_error("unimplemented when");
}

unique_ptr<Value> WheneverStatement::exec(Environment&) const {
  throw runtime_error("unimplemented whenever");
}

unique_ptr<Value> WhileStatement::exec(Environment& environment) const {
  while (true) {
    auto value(expression->eval(environment));
    value->assert_type(Value::BOOLEAN);
    auto condition(static_unique_cast<BooleanExpression>(move(value)));
    if (!condition->value)
      break;
    statement->exec(environment);
  }
  return unique_ptr<Value>(new UndefinedExpression());
}

unique_ptr<Value> RepeatWhenStatement::exec(Environment&) const {
  throw runtime_error("unimplemented repeat_when");
}

unique_ptr<Value> RepeatWheneverStatement::exec(Environment&) const {
  throw runtime_error("unimplemented repeat_whenever");
}

}
