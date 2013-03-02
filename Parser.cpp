#include "Parser.h"

#include "Expression.h"
#include "Statement.h"

#include <sstream>
#include <stdexcept>
#include <utility>

#include <tr1/memory>
#include <iostream>

using namespace std;
using namespace std::rel_ops;
using namespace std::tr1;

namespace hap {

shared_ptr<Statement> Parser::accept_program() {
  shared_ptr<Statement> statements(accept_statements());
  if (!at_end())
    expected("end of input");
  return statements;
}

shared_ptr<Statement> Parser::accept_statements() {
  shared_ptr<BlockStatement> block(new BlockStatement());
  shared_ptr<Statement> statement;
  while ((statement = accept_statement()))
    block->push(statement);
  return block;
}

shared_ptr<Statement> Parser::accept_statement() {
  shared_ptr<Statement> statement;
  (statement = accept_empty_statement())
    || (statement = accept_block_statement())
    || (statement = accept_var_statement())
    || (statement = accept_if_statement())
    || (statement = accept_when_statement())
    || (statement = accept_whenever_statement())
    || (statement = accept_while_statement())
    || (statement = accept_repeat_when_statement())
    || (statement = accept_repeat_whenever_statement());
  return statement;
}

shared_ptr<Statement> Parser::accept_empty_statement() {
  return accept(Token::SEMICOLON)
    ? shared_ptr<Statement>(new BlockStatement())
    : shared_ptr<Statement>();
}

shared_ptr<Statement> Parser::accept_block_statement() {
  if (!accept(Token::LEFT_BRACE))
    return shared_ptr<Statement>();
  shared_ptr<Statement> block = accept_statements();
  expect(Token::RIGHT_BRACE);
  return block;
}

shared_ptr<Statement> Parser::accept_var_statement() {
  if (!accept(Token(Token::IDENTIFIER, "var")))
    return shared_ptr<Statement>();
  Token identifier;
  expect(Token::IDENTIFIER, identifier);
  shared_ptr<Expression> initializer;
  if (accept(Token(Token::OPERATOR, "="))
      && !(initializer = accept_expression()))
    expected("expression");
  expect(Token::SEMICOLON);
  return shared_ptr<Statement>(new VarStatement(identifier.string, initializer));
}

shared_ptr<Statement> Parser::accept_if_statement() {
  return accept_flow_statement<IfStatement>("if");
}

shared_ptr<Statement> Parser::accept_when_statement() {
  return accept_flow_statement<WhenStatement>("when");
}

shared_ptr<Statement> Parser::accept_whenever_statement() {
  return accept_flow_statement<WheneverStatement>("whenever");
}

shared_ptr<Statement> Parser::accept_while_statement() {
  return accept_flow_statement<WhileStatement>("while");
}

shared_ptr<Statement> Parser::accept_repeat_when_statement() {
  return accept_flow_statement<RepeatWhenStatement>("repeat_when");
}

shared_ptr<Statement> Parser::accept_repeat_whenever_statement() {
  return accept_flow_statement<RepeatWheneverStatement>("repeat_whenever");
}

shared_ptr<Expression> Parser::accept_expression() {
  shared_ptr<Expression> expression;
  expression = accept_value_expression();
  if (!expression && accept(Token::LEFT_PARENTHESIS)) {
    expression = accept_expression();
    expect(Token::RIGHT_PARENTHESIS);
  }
  return expression;
}

shared_ptr<Expression> Parser::accept_value_expression() {
  shared_ptr<Expression> value;
  (value = accept_integer_expression())
    || (value = accept_identifier_expression())
    || (value = accept_list_expression());
  return value;
}

shared_ptr<Expression> Parser::accept_integer_expression() {
  Token token;
  if (!accept(Token::INTEGER, token))
    return shared_ptr<Expression>();
  istringstream stream(token.string);
  int value = 0;
  if (!(stream >> value))
    throw runtime_error("invalid integer");
  return shared_ptr<Expression>(new IntegerExpression(value));
}

shared_ptr<Expression> Parser::accept_identifier_expression() {
  Token token;
  if (!accept(Token::IDENTIFIER, token))
    return shared_ptr<Expression>();
  return shared_ptr<Expression>(new IdentifierExpression(token.string));
}

shared_ptr<Expression> Parser::accept_list_expression() {
  if (!accept(Token::LEFT_BRACKET))
    return shared_ptr<Expression>();
  shared_ptr<ListExpression> list(new ListExpression());
  shared_ptr<Expression> expression;
  while ((expression = accept_expression()))
    list->push(static_pointer_cast<Expression>(expression));
  expect(Token::RIGHT_BRACKET);
  return list;
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
