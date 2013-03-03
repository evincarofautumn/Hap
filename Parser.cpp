#include "Parser.h"

#include "Expression.h"
#include "Operator.h"
#include "Statement.h"

#include "unique_cast.h"

#include <map>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <memory>
#include <utility>

using namespace std;
using namespace std::rel_ops;

namespace hap {

unique_ptr<Statement> Parser::accept_program() {
  unique_ptr<Statement> statements(accept_statements());
  if (!at_end())
    expected("end of input");
  return statements;
}

unique_ptr<Statement> Parser::accept_statements() {
  unique_ptr<BlockStatement> block(new BlockStatement());
  unique_ptr<Statement> statement;
  while ((statement = accept_statement()))
    block->push(move(statement));
  return static_unique_cast<Statement>(move(block));
}

unique_ptr<Statement> Parser::accept_statement() {
  unique_ptr<Statement> statement;
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

unique_ptr<Statement> Parser::accept_empty_statement() {
  return accept(Token::SEMICOLON)
    ? unique_ptr<Statement>(new BlockStatement())
    : unique_ptr<Statement>();
}

unique_ptr<Statement> Parser::accept_block_statement() {
  if (!accept(Token::LEFT_BRACE))
    return unique_ptr<Statement>();
  unique_ptr<Statement> block(accept_statements());
  expect(Token::RIGHT_BRACE);
  return block;
}

unique_ptr<Statement> Parser::accept_var_statement() {
  if (!accept(Token(Token::IDENTIFIER, "var")))
    return unique_ptr<Statement>();
  Token identifier;
  expect(Token::IDENTIFIER, identifier);
  unique_ptr<Expression> initializer;
  if (accept(Token(Token::OPERATOR, "="))
      && !(initializer = accept_expression()))
    expected("expression");
  expect(Token::SEMICOLON);
  return unique_ptr<Statement>
    (new VarStatement(identifier.string, move(initializer)));
}

unique_ptr<Statement> Parser::accept_if_statement() {
  return accept_flow_statement<IfStatement>("if");
}

unique_ptr<Statement> Parser::accept_when_statement() {
  return accept_flow_statement<WhenStatement>("when");
}

unique_ptr<Statement> Parser::accept_whenever_statement() {
  return accept_flow_statement<WheneverStatement>("whenever");
}

unique_ptr<Statement> Parser::accept_while_statement() {
  return accept_flow_statement<WhileStatement>("while");
}

unique_ptr<Statement> Parser::accept_repeat_when_statement() {
  return accept_flow_statement<RepeatWhenStatement>("repeat_when");
}

unique_ptr<Statement> Parser::accept_repeat_whenever_statement() {
  return accept_flow_statement<RepeatWheneverStatement>("repeat_whenever");
}

unique_ptr<Expression> Parser::accept_value_expression() {
  unique_ptr<Expression> value;
  (value = accept_integer_expression())
    || (value = accept_identifier_expression())
    || (value = accept_list_expression());
  return value;
}

unique_ptr<Expression> Parser::accept_integer_expression() {
  Token token;
  if (!accept(Token::INTEGER, token))
    return unique_ptr<Expression>();
  istringstream stream(token.string);
  int value = 0;
  if (!(stream >> value))
    throw runtime_error("invalid integer");
  return unique_ptr<Expression>(new IntegerExpression(value));
}

unique_ptr<Expression> Parser::accept_identifier_expression() {
  Token token;
  if (!accept(Token::IDENTIFIER, token))
    return unique_ptr<Expression>();
  return unique_ptr<Expression>(new IdentifierExpression(token.string));
}

unique_ptr<Expression> Parser::accept_list_expression() {
  if (!accept(Token::LEFT_BRACKET))
    return unique_ptr<Expression>();
  unique_ptr<ListExpression> list(new ListExpression());
  unique_ptr<Expression> expression;
  while ((expression = accept_expression()))
    list->push(move(expression));
  expect(Token::RIGHT_BRACKET);
  return static_unique_cast<Expression>(move(list));
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

unique_ptr<Expression> Parser::accept_expression() {
  stack<Operator> operators;
  stack<unique_ptr<Expression>> operands;
  operators.push(Operator::SENTINEL);
  E(operators, operands);
  return move(operands.top());
}

void Parser::E
  (stack<Operator>& operators,
   stack<unique_ptr<Expression>>& operands) {
  P(operators, operands);
  Operator operator_;
  while (accept_binary_operator(operator_)) {
    push_operator(operator_, operators, operands);
    P(operators, operands);
  }
  while (operators.top().arity != Operator::SENTINEL)
    pop_operator(operators, operands);
}

void Parser::P
  (stack<Operator>& operators,
   stack<unique_ptr<Expression>>& operands) {
  Operator unary;
  unique_ptr<Expression> value;
  if ((value = accept_value_expression())) {
    operands.push(move(value));
  } else if (accept(Token::LEFT_PARENTHESIS)) {
    operators.push(Operator::SENTINEL);
    E(operators, operands);
    expect(Token::RIGHT_PARENTHESIS);
    operators.pop();
  } else if (accept_unary_operator(unary)) {
    push_operator(unary, operators, operands);
    P(operators, operands);
  } else {
    expected("expression");
  }
}

void Parser::pop_operator
  (stack<Operator>& operators,
   stack<unique_ptr<Expression>>& operands) {
  if (operators.top().arity == Operator::BINARY) {
    Operator operator_(operators.top());
    operators.pop();
    unique_ptr<Expression> b(move(operands.top()));
    operands.pop();
    unique_ptr<Expression> a(move(operands.top()));
    operands.pop();
    operands.push(unique_ptr<Expression>
      (new BinaryExpression(operator_, move(a), move(b))));
  } else if (operators.top().arity == Operator::UNARY) {
    Operator operator_(operators.top());
    operators.pop();
    unique_ptr<Expression> a(move(operands.top()));
    operands.pop();
    operands.push(unique_ptr<Expression>
      (new UnaryExpression(operator_, move(a))));
  } else {
    throw runtime_error("error parsing expression");
  }
}

void Parser::push_operator
  (const Operator& operator_,
   stack<Operator>& operators,
   stack<unique_ptr<Expression>>& operands) {
  while (operator_ < operators.top())
    pop_operator(operators, operands);
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
