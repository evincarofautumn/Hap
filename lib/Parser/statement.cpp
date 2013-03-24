#include "Parser.h"

#include "AtomicStatement.h"
#include "BlockStatement.h"
#include "Expression.h"
#include "ExpressionStatement.h"
#include "FlowStatement.h"
#include "FunExpression.h"
#include "FunStatement.h"
#include "LastStatement.h"
#include "NextStatement.h"
#include "RetStatement.h"
#include "TraceStatement.h"
#include "UndefinedValue.h"
#include "VarStatement.h"
#include "unique_cast.h"

using namespace std;

namespace hap {

shared_ptr<Statement>
Parser::accept_statements(const shared_ptr<Environment> environment) {
  shared_ptr<BlockStatement> block(new BlockStatement());
  shared_ptr<Statement> statement;
  shared_ptr<Environment> local(new Environment(environment));
  while ((statement = accept_statement(local)))
    block->push(statement);
  return static_pointer_cast<Statement>(move(block));
}

shared_ptr<Statement>
Parser::accept_statement(const shared_ptr<Environment> environment) {
  return first<Statement>
    (environment,
     &Parser::accept_atomic_statement,
     &Parser::accept_block_statement,
     &Parser::accept_empty_statement,
     &Parser::accept_fun_statement,
     &Parser::accept_if_statement,
     &Parser::accept_last_statement,
     &Parser::accept_next_statement,
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
    return shared_ptr<Statement>();
  auto statement(accept_statement(environment));
  if (!statement)
    expected("statement");
  return shared_ptr<Statement>(new AtomicStatement(statement));
}

shared_ptr<Statement>
Parser::accept_block_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token::LEFT_BRACE))
    return shared_ptr<Statement>();
  shared_ptr<Statement> block(accept_statements(environment));
  expect(Token::RIGHT_BRACE);
  return block;
}

shared_ptr<Statement>
Parser::accept_empty_statement(const shared_ptr<Environment>) {
  return accept(Token::SEMICOLON)
    ? shared_ptr<Statement>(new BlockStatement())
    : shared_ptr<Statement>();
}

shared_ptr<Statement>
Parser::accept_expression_statement
  (const shared_ptr<Environment> environment) {
  auto expression(accept_expression(environment));
  if (!expression)
    return shared_ptr<Statement>();
  expect(Token::SEMICOLON);
  return shared_ptr<Statement>(new ExpressionStatement(move(expression)));
}

shared_ptr<Statement>
Parser::accept_fun_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "fun")))
    return shared_ptr<Statement>();
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
  shared_ptr<Statement> statement
    (new FunStatement(identifier.string, parameters, move(body), environment));
  return move(statement);
}

shared_ptr<Statement>
Parser::accept_if_statement(const shared_ptr<Environment> environment) {
  return accept_flow_statement<IfStatement>
    (environment, "if");
}

shared_ptr<Statement>
Parser::accept_last_statement(const shared_ptr<Environment> environment) {
  return accept(Token(Token::IDENTIFIER, "last"))
    ? shared_ptr<Statement>(new LastStatement())
    : shared_ptr<Statement>();
}

shared_ptr<Statement>
Parser::accept_next_statement(const shared_ptr<Environment> environment) {
  return accept(Token(Token::IDENTIFIER, "next"))
    ? shared_ptr<Statement>(new NextStatement())
    : shared_ptr<Statement>();
}

shared_ptr<Statement>
Parser::accept_ret_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "ret")))
    return shared_ptr<Statement>();
  auto expression(accept_expression(environment));
  return expression
    ? shared_ptr<Statement>(new RetStatement(move(expression)))
    : shared_ptr<Statement>();
}

shared_ptr<Statement>
Parser::accept_trace_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "trace")))
    return shared_ptr<Statement>();
  auto expression(accept_expression(environment));
  return expression
    ? shared_ptr<Statement>(new TraceStatement(move(expression)))
    : shared_ptr<Statement>();
}

shared_ptr<Statement>
Parser::accept_var_statement(const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "var")))
    return shared_ptr<Statement>();
  Token identifier;
  expect(Token::IDENTIFIER, identifier);
  shared_ptr<Expression> initializer(new UndefinedValue());
  if (accept(Token(Token::OPERATOR, "="))
      && !(initializer = accept_expression(environment)))
    expected("initializer");
  expect(Token::SEMICOLON);
  shared_ptr<Statement> statement
    (new VarStatement(identifier.string, move(initializer)));
  return move(statement);
}

shared_ptr<Statement>
Parser::accept_when_statement(const shared_ptr<Environment> environment) {
  return accept_flow_statement<WhenStatement>
    (environment, "when");
}

shared_ptr<Statement>
Parser::accept_whenever_statement(const shared_ptr<Environment> environment) {
  return accept_flow_statement<WheneverStatement>
    (environment, "whenever");
}

shared_ptr<Statement>
Parser::accept_while_statement(const shared_ptr<Environment> environment) {
  return accept_flow_statement<WhileStatement>
    (environment, "while");
}

}