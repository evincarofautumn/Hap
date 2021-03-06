#include "Parser.h"

#include "AtomicStatement.h"
#include "BlockStatement.h"
#include "ControlStatement.h"
#include "DelStatement.h"
#include "Expression.h"
#include "ExpressionStatement.h"
#include "FlowStatement.h"
#include "ForStatement.h"
#include "FunStatement.h"
#include "FunValue.h"
#include "RetStatement.h"
#include "TraceStatement.h"
#include "UndefinedValue.h"
#include "VarStatement.h"

using namespace std;

namespace hap {

shared_ptr<Statement>
Parser::accept_statements(const shared_ptr<Environment> environment) {
  const auto block = make_shared<BlockStatement>();
  shared_ptr<Statement> statement;
  const auto local = make_shared<Environment>(environment);
  while ((statement = accept_statement(local)))
    block->push(statement);
  return static_pointer_cast<Statement>(block);
}

shared_ptr<Statement>
Parser::accept_statement(const shared_ptr<Environment> environment) {
  return first<Statement>
    (environment,
     &Parser::accept_atomic_statement,
     &Parser::accept_block_statement,
     &Parser::accept_del_statement,
     &Parser::accept_empty_statement,
     &Parser::accept_exit_statement,
     &Parser::accept_for_statement,
     &Parser::accept_fun_statement,
     &Parser::accept_if_statement,
     &Parser::accept_last_statement,
     &Parser::accept_next_statement,
     &Parser::accept_redo_statement,
     &Parser::accept_ret_statement,
     &Parser::accept_trace_statement,
     &Parser::accept_var_statement,
     &Parser::accept_when_statement,
     &Parser::accept_whenever_statement,
     &Parser::accept_while_statement,
     // ----
     &Parser::accept_expression_statement);
}

shared_ptr<Statement>
Parser::accept_atomic_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "atomic")))
    return nullptr;
  auto statement(accept_statement(environment));
  if (!statement)
    expected("statement");
  return make_shared<AtomicStatement>(statement);
}

shared_ptr<Statement>
Parser::accept_block_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token::LEFT_BRACE))
    return nullptr;
  shared_ptr<Statement> block(accept_statements(environment));
  expect(Token::RIGHT_BRACE);
  return block;
}

shared_ptr<Statement>
Parser::accept_del_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "del")))
    return nullptr;
  Token identifier;
  expect(Token::IDENTIFIER, identifier);
  expect(Token::SEMICOLON);
  return make_shared<DelStatement>(identifier.string);
}

shared_ptr<Statement>
Parser::accept_empty_statement(const shared_ptr<Environment>) {
  return accept(Token::SEMICOLON) ? make_shared<BlockStatement>() : nullptr;
}

shared_ptr<Statement>
Parser::accept_exit_statement(const shared_ptr<Environment> environment) {
  return accept_control_statement<ExitStatement>(environment, "exit");
}

shared_ptr<Statement>
Parser::accept_expression_statement
  (const shared_ptr<Environment> environment) {
  auto expression(accept_expression(environment));
  if (!expression)
    return nullptr;
  expect(Token::SEMICOLON);
  return make_shared<ExpressionStatement>(expression);
}

shared_ptr<Statement> Parser::accept_for_statement
  (const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "for")))
    return nullptr;
  expect(Token::LEFT_PARENTHESIS);
  const auto initializer(accept_statement(environment));
  if (!initializer)
    expected("for loop initializer");
  const auto condition(accept_expression(environment));
  if (!condition)
    expected("for loop condition");
  expect(Token::SEMICOLON);
  const auto step(accept_expression(environment));
  if (!step)
    expected("for loop step");
  expect(Token::RIGHT_PARENTHESIS);
  const auto body(accept_statement(environment));
  if (!body)
    expected("statement");
  return make_shared<ForStatement>(initializer, condition, step, body);
}

shared_ptr<Statement>
Parser::accept_fun_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "fun")))
    return nullptr;
  Token identifier;
  expect(Token::IDENTIFIER, identifier);
  vector<string> parameters;
  expect(Token::LEFT_PARENTHESIS);
  if (!peek(Token::RIGHT_PARENTHESIS)) {
    while (true) {
      if (peek(Token::RIGHT_PARENTHESIS))
        break;
      Token parameter;
      expect(Token::IDENTIFIER, parameter);
      parameters.push_back(move(parameter.string));
      if (!accept(Token::COMMA))
        break;
    }
  }
  expect(Token::RIGHT_PARENTHESIS);
  shared_ptr<Statement> body(accept_statement(environment));
  if (!body)
    expected("statement");
  return make_shared<FunStatement>
    (identifier.string, parameters, body, environment);
}

shared_ptr<Statement>
Parser::accept_if_statement(const shared_ptr<Environment> environment) {
  return accept_flow_statement<IfStatement>(environment, "if");
}

shared_ptr<Statement>
Parser::accept_last_statement(const shared_ptr<Environment> environment) {
  return accept_control_statement<LastStatement>(environment, "last");
}

shared_ptr<Statement>
Parser::accept_next_statement(const shared_ptr<Environment> environment) {
  return accept_control_statement<NextStatement>(environment, "next");
}

shared_ptr<Statement>
Parser::accept_redo_statement(const shared_ptr<Environment> environment) {
  return accept_control_statement<RedoStatement>(environment, "redo");
}

shared_ptr<Statement>
Parser::accept_ret_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "ret")))
    return nullptr;
  auto expression(accept_expression(environment));
  expect(Token::SEMICOLON);
  if (!expression)
    return make_shared<RetStatement>
      (make_shared<UndefinedValue>());
  return make_shared<RetStatement>(expression);
}

shared_ptr<Statement>
Parser::accept_trace_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "trace")))
    return nullptr;
  auto expression(accept_expression(environment));
  return expression ? make_shared<TraceStatement>(expression) : nullptr;
}

shared_ptr<Statement>
Parser::accept_var_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "var")))
    return nullptr;
  Token identifier;
  expect(Token::IDENTIFIER, identifier);
  shared_ptr<Expression> initializer = make_shared<UndefinedValue>();
  if (accept(Token(Token::OPERATOR, "="))
      && !(initializer = accept_expression(environment)))
    expected("initializer");
  expect(Token::SEMICOLON);
  return make_shared<VarStatement>(identifier.string, initializer);
}

shared_ptr<Statement>
Parser::accept_when_statement(const shared_ptr<Environment> environment) {
  return accept_flow_statement<WhenStatement>(environment, "when");
}

shared_ptr<Statement>
Parser::accept_whenever_statement(const shared_ptr<Environment> environment) {
  return accept_flow_statement<WheneverStatement>(environment, "whenever");
}

shared_ptr<Statement>
Parser::accept_while_statement(const shared_ptr<Environment> environment) {
  return accept_flow_statement<WhileStatement>(environment, "while");
}

}
