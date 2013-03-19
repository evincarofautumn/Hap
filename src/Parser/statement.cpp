#include "Parser.h"

#include "BlockStatement.h"
#include "Expression.h"
#include "ExpressionStatement.h"
#include "FlowStatement.h"
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

unique_ptr<Statement>
Parser::accept_statements(Environment& environment) {
  unique_ptr<BlockStatement> block(new BlockStatement());
  unique_ptr<Statement> statement;
  Environment local(&environment);
  while ((statement = accept_statement(local)))
    block->push(move(statement));
  return static_unique_cast<Statement>(move(block));
}

unique_ptr<Statement>
Parser::accept_statement(Environment& environment) {
  return first<Statement>
    (environment,
     &Parser::accept_block_statement,
     &Parser::accept_empty_statement,
     &Parser::accept_fun_statement,
     &Parser::accept_if_statement,
     &Parser::accept_last_statement,
     &Parser::accept_next_statement,
     &Parser::accept_repeat_when_statement,
     &Parser::accept_repeat_whenever_statement,
     &Parser::accept_ret_statement,
     &Parser::accept_trace_statement,
     &Parser::accept_var_statement,
     &Parser::accept_when_statement,
     &Parser::accept_whenever_statement,
     &Parser::accept_while_statement,
     // ----
     &Parser::accept_expression_statement);
}

unique_ptr<Statement>
Parser::accept_block_statement(Environment& environment) {
  if (!accept(Token::LEFT_BRACE))
    return unique_ptr<Statement>();
  unique_ptr<Statement> block(accept_statements(environment));
  expect(Token::RIGHT_BRACE);
  return block;
}

unique_ptr<Statement>
Parser::accept_empty_statement(Environment&) {
  return accept(Token::SEMICOLON)
    ? unique_ptr<Statement>(new BlockStatement())
    : unique_ptr<Statement>();
}

unique_ptr<Statement>
Parser::accept_expression_statement(Environment& environment) {
  auto expression(accept_expression(environment));
  if (!expression)
    return unique_ptr<Statement>();
  expect(Token::SEMICOLON);
  return unique_ptr<Statement>(new ExpressionStatement(move(expression)));
}

unique_ptr<Statement>
Parser::accept_fun_statement(Environment& environment) {
  if (!accept(Token(Token::IDENTIFIER, "fun")))
    return unique_ptr<Statement>();
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
  unique_ptr<Statement> body(accept_statement(environment));
  if (!body)
    expected("statement");
  return unique_ptr<Statement>
    (new FunStatement(identifier.string, parameters, move(body), environment));
}

unique_ptr<Statement>
Parser::accept_if_statement(Environment& environment) {
  return accept_flow_statement<IfStatement>
    (environment, "if");
}

unique_ptr<Statement>
Parser::accept_last_statement(Environment& environment) {
  return accept(Token(Token::IDENTIFIER, "last"))
    ? unique_ptr<Statement>(new LastStatement())
    : unique_ptr<Statement>();
}

unique_ptr<Statement>
Parser::accept_next_statement(Environment& environment) {
  return accept(Token(Token::IDENTIFIER, "next"))
    ? unique_ptr<Statement>(new NextStatement())
    : unique_ptr<Statement>();
}

unique_ptr<Statement>
Parser::accept_repeat_when_statement(Environment& environment) {
  return accept_flow_statement<RepeatWhenStatement>
    (environment, "repeat_when");
}

unique_ptr<Statement>
Parser::accept_repeat_whenever_statement(Environment& environment) {
  return accept_flow_statement<RepeatWheneverStatement>
    (environment, "repeat_whenever");
}

unique_ptr<Statement>
Parser::accept_ret_statement(Environment& environment) {
  if (!accept(Token(Token::IDENTIFIER, "ret")))
    return unique_ptr<Statement>();
  auto expression(accept_expression(environment));
  return expression
    ? unique_ptr<Statement>(new RetStatement(move(expression)))
    : unique_ptr<Statement>();
}

unique_ptr<Statement>
Parser::accept_trace_statement(Environment& environment) {
  if (!accept(Token(Token::IDENTIFIER, "trace")))
    return unique_ptr<Statement>();
  auto expression(accept_expression(environment));
  return expression
    ? unique_ptr<Statement>(new TraceStatement(move(expression)))
    : unique_ptr<Statement>();
}

unique_ptr<Statement>
Parser::accept_var_statement(Environment& environment) {
  if (!accept(Token(Token::IDENTIFIER, "var")))
    return unique_ptr<Statement>();
  Token identifier;
  expect(Token::IDENTIFIER, identifier);
  unique_ptr<Expression> initializer(new UndefinedValue());
  if (accept(Token(Token::OPERATOR, "="))
      && !(initializer = accept_expression(environment)))
    expected("initializer");
  expect(Token::SEMICOLON);
  return unique_ptr<Statement>
    (new VarStatement(identifier.string, move(initializer)));
}

unique_ptr<Statement>
Parser::accept_when_statement(Environment& environment) {
  return accept_flow_statement<WhenStatement>
    (environment, "when");
}

unique_ptr<Statement>
Parser::accept_whenever_statement(Environment& environment) {
  return accept_flow_statement<WheneverStatement>
    (environment, "whenever");
}

unique_ptr<Statement>
Parser::accept_while_statement(Environment& environment) {
  return accept_flow_statement<WhileStatement>
    (environment, "while");
}

}
