#include "Parser.h"

#include "Expression.h"
#include "Operator.h"
#include "Statement.h"
#include "binary.h"
#include "unary.h"

#include "unique_cast.h"

#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <stdexcept>
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
  return first<Statement>
    (&Parser::accept_empty_statement,
     &Parser::accept_block_statement,
     &Parser::accept_var_statement,
     &Parser::accept_fun_statement,
     &Parser::accept_ret_statement,
     &Parser::accept_if_statement,
     &Parser::accept_when_statement,
     &Parser::accept_whenever_statement,
     &Parser::accept_while_statement,
     &Parser::accept_repeat_when_statement,
     &Parser::accept_repeat_whenever_statement);
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
    expected("initializer");
  expect(Token::SEMICOLON);
  return unique_ptr<Statement>
    (new VarStatement(identifier.string, move(initializer)));
}

unique_ptr<Statement> Parser::accept_fun_statement() {
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
  unique_ptr<Statement> body(accept_statement());
  if (!body)
    expected("statement");
  return unique_ptr<Statement>
    (new FunStatement(identifier.string, parameters, move(body)));
}

unique_ptr<Statement> Parser::accept_ret_statement() {
  if (!accept(Token(Token::IDENTIFIER, "ret")))
    return unique_ptr<Statement>();
  auto expression(accept_expression());
  return expression
    ? unique_ptr<Statement>(new RetStatement(move(expression)))
    : unique_ptr<Statement>();
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
  return first<Expression>
    (&Parser::accept_integer_expression,
     &Parser::accept_boolean_expression,
     &Parser::accept_identifier_expression,
     &Parser::accept_string_expression,
     &Parser::accept_lambda_expression);
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

unique_ptr<Expression> Parser::accept_boolean_expression() {
  if (accept(Token(Token::IDENTIFIER, "true"))) {
    return unique_ptr<Expression>(new BooleanExpression(true));
  } else if (accept(Token(Token::IDENTIFIER, "false"))) {
    return unique_ptr<Expression>(new BooleanExpression(false));
  } else {
    return unique_ptr<Expression>();
  }
}

unique_ptr<Expression> Parser::accept_identifier_expression() {
  Token token;
  if (!accept(Token::IDENTIFIER, token))
    return unique_ptr<Expression>();
  return unique_ptr<Expression>(new IdentifierExpression(token.string));
}

unique_ptr<Expression> Parser::accept_string_expression() {
  Token token;
  if (!accept(Token::STRING, token))
    return unique_ptr<Expression>();
  token.string.pop_back();
  return unique_ptr<Expression>(new StringExpression(token.string.substr(1)));
}

unique_ptr<Expression> Parser::accept_lambda_expression() {
  if (!accept(Token(Token::OPERATOR, "\\")))
    return unique_ptr<Expression>();
  Token identifier(Token::IDENTIFIER, "lambda");
  accept(Token::IDENTIFIER, identifier);
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
  unique_ptr<Statement> body;
  if (accept(Token::COLON)) {
    auto expression(accept_expression());
    if (!expression)
      expected("expression");
    body.reset(new RetStatement(move(expression)));
  } else if (accept(Token::LEFT_BRACE)) {
    body = accept_statements();
    expect(Token::RIGHT_BRACE);
  } else {
    expected("colon or block");
  }
  Environment environment;
  return unique_ptr<Expression>
    (new FunExpression
     (identifier.string,
      parameters,
      move(body),
      environment));
}

map<string, Operator> binary_operators;
map<string, Operator> unary_operators;

#define BINARY_OPERATOR(NAME, ASSOCIATIVITY, PRECEDENCE, IMPLEMENTATION) \
  binary_operators.insert(make_pair(NAME, \
    Operator(Operator::BINARY, NAME, \
      Operator::ASSOCIATIVITY, Operator::PRECEDENCE, IMPLEMENTATION)))

bool Parser::accept_binary_operator(Operator& result) {
  if (binary_operators.empty()) {
    using namespace binary;
    BINARY_OPERATOR("*", LEFT, MULTIPLICATIVE, multiply);
    BINARY_OPERATOR("/", LEFT, MULTIPLICATIVE, divide);
    BINARY_OPERATOR("mod", LEFT, MULTIPLICATIVE, modulate);
    BINARY_OPERATOR("+", LEFT, ADDITIVE, add);
    BINARY_OPERATOR("-", LEFT, ADDITIVE, subtract);
    BINARY_OPERATOR("<<", LEFT, SHIFTING, shift_left);
    BINARY_OPERATOR(">>", LEFT, SHIFTING, shift_right);
    BINARY_OPERATOR("<", LEFT, RELATIONAL, lt);
    BINARY_OPERATOR(">=", LEFT, RELATIONAL, ge);
    BINARY_OPERATOR(">", LEFT, RELATIONAL, gt);
    BINARY_OPERATOR("<=", LEFT, RELATIONAL, le);
    BINARY_OPERATOR("==", LEFT, RELATIONAL, eq);
    BINARY_OPERATOR("<>", LEFT, RELATIONAL, ne);
    BINARY_OPERATOR("and", LEFT, AND, and_);
    BINARY_OPERATOR("xor", LEFT, XOR, xor_);
    BINARY_OPERATOR("or", LEFT, OR, or_);
    BINARY_OPERATOR("=", RIGHT, ASSIGNMENT, assign);
    BINARY_OPERATOR(",", RIGHT, COMMA, comma);
  }
  if (at_end() || !(current->type == Token::OPERATOR
      || current->type == Token::IDENTIFIER))
    return false;
  map<string, Operator>::const_iterator operator_
    (binary_operators.find(current->string));
  if (operator_ == binary_operators.end())
    return false;
  ++current;
  result = operator_->second;
  return true;
}

#undef BINARY_OPERATOR

#define UNARY_OPERATOR(NAME, IMPLEMENTATION) \
  unary_operators.insert(make_pair(NAME, \
    Operator(Operator::UNARY, NAME, Operator::RIGHT, Operator::TIGHTEST, \
      IMPLEMENTATION)))

bool Parser::accept_unary_operator(Operator& result) {
  if (unary_operators.empty()) {
    using namespace unary;
    UNARY_OPERATOR("+", identity);
    UNARY_OPERATOR("-", negate);
    UNARY_OPERATOR("val", val);
    UNARY_OPERATOR("ref", ref);
    UNARY_OPERATOR("not", not_);
    UNARY_OPERATOR("<", lt);
    UNARY_OPERATOR(">=", ge);
    UNARY_OPERATOR(">", gt);
    UNARY_OPERATOR("<=", le);
    UNARY_OPERATOR("==", eq);
    UNARY_OPERATOR("<>", ne);
  }
  if (at_end() || !(current->type == Token::OPERATOR
      || current->type == Token::IDENTIFIER))
    return false;
  map<string, Operator>::const_iterator operator_
    (unary_operators.find(current->string));
  if (operator_ == unary_operators.end())
    return false;
  ++current;
  result = operator_->second;
  return true;
}

#undef UNARY_OPERATOR

unique_ptr<Expression> Parser::accept_expression() {
  stack<Operator> operators;
  stack<unique_ptr<Expression>> operands;
  operators.push(Operator());
  infix_expression(operators, operands);
  return move(operands.top());
}

void Parser::infix_expression
  (stack<Operator>& operators,
   stack<unique_ptr<Expression>>& operands) {
  infix_subexpression(operators, operands);
  Operator operator_;
  while (accept_binary_operator(operator_)) {
    push_operator(operator_, operators, operands);
    infix_subexpression(operators, operands);
  }
  while (operators.top().arity != Operator::SENTINEL)
    pop_operator(operators, operands);
}

void Parser::infix_subexpression
  (stack<Operator>& operators,
   stack<unique_ptr<Expression>>& operands) {
  Operator unary;
  unique_ptr<Expression> value;
  if (accept(Token::LEFT_PARENTHESIS)) {
    operators.push(Operator());
    infix_expression(operators, operands);
    expect(Token::RIGHT_PARENTHESIS);
    operators.pop();
  } else if (accept(Token::LEFT_BRACKET)) {
    unique_ptr<ListExpression> list(new ListExpression());
    operators.push(Operator());
    if (!peek(Token::RIGHT_BRACKET)) {
      while (true) {
        if (peek(Token::RIGHT_BRACKET))
          break;
        infix_expression(operators, operands);
        list->push(move(operands.top()));
        operands.pop();
        if (!accept(Token::COMMA))
          break;
      }
    }
    expect(Token::RIGHT_BRACKET);
    operators.pop();
    operands.push(static_unique_cast<Expression>(move(list)));
  } else if (accept(Token::LEFT_BRACE)) {
    unique_ptr<MapExpression> map(new MapExpression());
    operators.push(Operator());
    if (!peek(Token::RIGHT_BRACE)) {
      while (true) {
        if (peek(Token::RIGHT_BRACE))
          break;
        unique_ptr<const Expression> key;
        Token bareword;
        if (accept(Token::IDENTIFIER, bareword)) {
          key.reset(new StringExpression(bareword.string));
        } else {
          infix_expression(operators, operands);
          key = move(operands.top());
          operands.pop();
        }
        expect(Token::COLON);
        infix_expression(operators, operands);
        unique_ptr<const Expression> value(move(operands.top()));
        operands.pop();
        map->insert(move(key), move(value));
        if (!accept(Token::COMMA))
          break;
      }
    }
    expect(Token::RIGHT_BRACE);
    operators.pop();
    operands.push(static_unique_cast<Expression>(move(map)));
  } else if (accept_unary_operator(unary)) {
    push_operator(unary, operators, operands);
    infix_subexpression(operators, operands);
  } else if ((value = accept_value_expression())) {
    operands.push(move(value));
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

bool Parser::peek(Token::Type type) const {
  return !at_end() && current->type == type;
}

bool Parser::peek(const Token& token) const {
  return !at_end() && *current == token;
}

bool Parser::accept(Token::Type type) {
  if (!peek(type))
    return false;
  ++current;
  return true;
}

bool Parser::accept(Token::Type type, Token& result) {
  if (!peek(type))
    return false;
  result = *current++;
  return true;
}

bool Parser::accept(const Token& token) {
  if (!peek(token))
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
