#include "Parser.h"

#include "Expression.h"
#include "Operator.h"
#include "Statement.h"

#include <map>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <tr1/memory>
#include <utility>

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

map<string, Operator> binary_operators;
map<string, Operator> unary_operators;

bool Parser::accept_binary_operator(Operator& result) {
  if (binary_operators.empty()) {
    map<string, Operator>& _(binary_operators);
    _["+"] = Operator(Operator::BINARY, "+", Operator::LEFT, 6);
    _["-"] = Operator(Operator::BINARY, "-", Operator::LEFT, 6);
    _["*"] = Operator(Operator::BINARY, "*", Operator::LEFT, 7);
    _["/"] = Operator(Operator::BINARY, "/", Operator::LEFT, 7);
  }
  if (at_end() || current->type != Token::OPERATOR)
    return false;
  map<string, Operator>::const_iterator operator_
    (binary_operators.find(current->string));
  if (operator_ == binary_operators.end())
    return false;
  ++current;
  result = operator_->second;
  return true;
}

bool Parser::accept_unary_operator(Operator& result) {
  if (unary_operators.empty()) {
    map<string, Operator>& _(unary_operators);
    _["+"] = Operator(Operator::UNARY, "+", Operator::RIGHT, 8);
    _["-"] = Operator(Operator::UNARY, "-", Operator::RIGHT, 8);
    _["*"] = Operator(Operator::UNARY, "*", Operator::RIGHT, 8);
  }
  if (at_end() || current->type != Token::OPERATOR)
    return false;
  map<string, Operator>::const_iterator operator_
    (unary_operators.find(current->string));
  if (operator_ == unary_operators.end())
    return false;
  ++current;
  result = operator_->second;
  return true;
}

shared_ptr<Expression> Parser::accept_expression() {
  // var operators : Stack of Operator := empty
  stack< Operator > operators;
  // var operands : Stack of Tree := empty
  stack< shared_ptr<Expression> > operands;
  // push( operators, sentinel )
  operators.push(Operator::SENTINEL);
  // E( operators, operands )
  E(operators, operands);
  // expect( end )
  // return top( operands )
  return operands.top();
}

void Parser::E
  (stack<Operator>& operators,
   stack< shared_ptr<Expression> >& operands) {
  // P( operators, operands )
  P(operators, operands);
  // while next is a binary operator
  Operator operator_;
  while (accept_binary_operator(operator_)) {
  //    pushOperator( binary(next), operators, operands )
    pushOperator(operator_, operators, operands);
  //    consume
  //    P( operators, operands )
    P(operators, operands);
  }
  // while top(operators) not= sentinel
  while (operators.top().arity != Operator::SENTINEL)
  //    popOperator( operators, operands )
    popOperator(operators, operands);
}

void Parser::P
  (stack<Operator>& operators,
   stack< shared_ptr<Expression> >& operands) {
  Operator unary;
  // if next is a v
  shared_ptr<Expression> value;
  if ((value = accept_value_expression())) {
  //      push( operands, mkLeaf( v ) )
    operands.push(value);
  //      consume
  // else if next = "("
  } else if (accept(Token::LEFT_PARENTHESIS)) {
  //      consume
  //      push( operators, sentinel )
    operators.push(Operator::SENTINEL);
  //      E( operators, operands )
    E(operators, operands);
  //      expect( ")" )
    expect(Token::RIGHT_PARENTHESIS);
  //      pop( operators )
    operators.pop();
  // else if next is a unary operator
  } else if (accept_unary_operator(unary)) {
  //      pushOperator( unary(next), operators, operands )
    pushOperator(unary, operators, operands);
  //      consume
  //      P( operators, operands )
    P(operators, operands);
  // else
  } else {
  //      error
    expected("expression");
  }
}

void Parser::popOperator
  (stack<Operator>& operators,
   stack< shared_ptr<Expression> >& operands) {
  // if top(operators) is binary
  if (operators.top().arity == Operator::BINARY) {
    Operator operator_(operators.top());
    operators.pop();
  //      const t1 := pop( operands )
    shared_ptr<Expression> const b(operands.top());
    operands.pop();
  //      const t0 := pop( operands )
    shared_ptr<Expression> const a(operands.top());
    operands.pop();
  //      push( operands, mkNode( pop(operators), t0, t1 ) )
    operands.push(shared_ptr<Expression>(new BinaryExpression(operator_, a, b)));
  // else
  } else if (operators.top().arity == Operator::UNARY) {
    Operator operator_(operators.top());
    operators.pop();
  //      push( operands, mkNode( pop(operators), pop(operands) ) )
    shared_ptr<Expression> const a(operands.top());
    operands.pop();
    operands.push(shared_ptr<Expression>(new UnaryExpression(operator_, a)));
  } else {
    throw runtime_error("error parsing expression");
  }
}

void Parser::pushOperator
  (const Operator& operator_,
   stack<Operator>& operators,
   stack< shared_ptr<Expression> >& operands) {
  // while top(operators) > operator_
  while (operator_ < operators.top())
  //    popOperator( operators, operands )
    popOperator(operators, operands);
  // push( operator_, operators )
  operators.push(operator_);
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
