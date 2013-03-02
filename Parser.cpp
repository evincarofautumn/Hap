#include "Parser.h"

#include "Expression.h"
#include "Statement.h"

#include <sstream>
#include <stdexcept>
#include <utility>

#include <iostream>

using namespace std;
using namespace std::rel_ops;

namespace hap {

Statement* Parser::accept_program() {
  Statement* statements = accept_statements();
  if (!at_end())
    expected("end of input");
  return statements;
}

Statement* Parser::accept_statements() {
  BlockStatement* block = new BlockStatement();
  Statement* statement = 0;
  while ((statement = accept_statement()))
    block->push(statement);
  return block;
}

Statement* Parser::accept_statement() {
  Statement* statement = 0;
  (statement = accept_empty_statement())
    || (statement = accept_block_statement())
    || (statement = accept_var_statement())
    || (statement = accept_if_statement())
    || (statement = accept_when_statement())
    || (statement = accept_whenever_statement())
    || (statement = accept_while_statement())
    || (statement = accept_repeat_when_statement())
    || (statement = accept_repeat_whenever_statement())
    ;
  return statement;
}

Statement* Parser::accept_empty_statement() {
  return accept(Token::SEMICOLON) ? new BlockStatement() : 0;
}

Statement* Parser::accept_block_statement() {
  if (!accept(Token::LEFT_BRACE))
    return 0;
  Statement* block = accept_statements();
  expect(Token::RIGHT_BRACE);
  return block;
}

Statement* Parser::accept_var_statement() {
  if (!accept(Token(Token::IDENTIFIER, "var")))
    return 0;
  Token identifier;
  expect(Token::IDENTIFIER, identifier);
  Expression* initializer = 0;
  if (accept(Token(Token::OPERATOR, "="))
      && !(initializer = accept_expression()))
    expected("expression");
  expect(Token::SEMICOLON);
  return new VarStatement(identifier.string, initializer);
}

Statement* Parser::accept_if_statement() {
  return accept_flow_statement<IfStatement>("if");
}

Statement* Parser::accept_when_statement() {
  return accept_flow_statement<WhenStatement>("when");
}

Statement* Parser::accept_whenever_statement() {
  return accept_flow_statement<WheneverStatement>("whenever");
}

Statement* Parser::accept_while_statement() {
  return accept_flow_statement<WhileStatement>("while");
}

Statement* Parser::accept_repeat_when_statement() {
  return accept_flow_statement<RepeatWhenStatement>("repeat_when");
}

Statement* Parser::accept_repeat_whenever_statement() {
  return accept_flow_statement<RepeatWheneverStatement>("repeat_whenever");
}

Expression* Parser::accept_expression() {
  Expression* expression = 0;
  expression = accept_value_expression();
  if (!expression && accept(Token::LEFT_PARENTHESIS)) {
    expression = accept_expression();
    expect(Token::RIGHT_PARENTHESIS);
  }
  return expression;
}

Expression* Parser::accept_value_expression() {
  Expression* value = 0;
  (value = accept_integer_expression())
    || (value = accept_identifier_expression());
  return value;
}

Expression* Parser::accept_integer_expression() {
  Token token;
  if (!accept(Token::INTEGER, token))
    return 0;
  istringstream stream(token.string);
  int value = 0;
  if (!(stream >> value))
    throw runtime_error("invalid integer");
  return new IntegerExpression(value);
}

Expression* Parser::accept_identifier_expression() {
  Token token;
  if (!accept(Token::IDENTIFIER, token))
    return 0;
  return new IdentifierExpression(token.string);
}

bool Parser::accept(Token::Type type) {
  if (at_end() || current->type != type)
    return false;
  ++current;
  return true;
}

bool Parser::accept(Token::Type type, Token& result) {
  if (at_end() || current->type != type)
    return false;
  result = *current++;
  return true;
}

bool Parser::accept(const Token& token) {
  if (at_end() || *current != token)
    return false;
  ++current;
  return true;
}

void Parser::expect(Token::Type type) {
  if (!accept(type))
    expected(type);
}

void Parser::expect(Token::Type type, Token& result) {
  if (!accept(type, result))
    expected(type);
}

void Parser::expect(const Token& token) {
  if (!accept(token))
    expected(token);
}

void Parser::expected(const std::string& abstract) const {
  ostringstream message;
  message << "expected " << abstract << " but got ";
  if (at_end()) {
    message << "end of input";
  } else {
    message << *current;
  }
  throw runtime_error(message.str());
}

void Parser::expected(Token::Type type) const {
  ostringstream message;
  message << "expected " << type << " but got ";
  if (at_end()) {
    message << "end of input";
  } else {
    message << *current;
  }
  throw runtime_error(message.str());
}

void Parser::expected(const Token& token) const {
  ostringstream message;
  message << "expected " << token << " but got ";
  if (at_end()) {
    message << "end of input";
  } else {
    message << *current;
  }
  throw runtime_error(message.str());
}

bool Parser::at_end() const {
  return current == tokens.end();
}

}
