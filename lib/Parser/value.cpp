#include "Parser.h"

#include "BooleanValue.h"
#include "FloatValue.h"
#include "FunValue.h"
#include "IntegerValue.h"
#include "RetStatement.h"
#include "StringValue.h"
#include "UndefinedValue.h"

#include <memory>
#include <sstream>

using namespace std;

namespace hap {

shared_ptr<Expression> Parser::accept_boolean_value
  (const shared_ptr<Environment>) {
  if (accept(Token(Token::IDENTIFIER, "true")))
    return shared_ptr<Expression>(new BooleanValue(true));
  if (accept(Token(Token::IDENTIFIER, "false")))
    return shared_ptr<Expression>(new BooleanValue(false));
  return shared_ptr<Expression>();
}

shared_ptr<Expression> Parser::accept_float_value
  (const shared_ptr<Environment> environment) {
  Token token;
  if (!accept(Token::FLOAT, token))
    return shared_ptr<Expression>();
  istringstream stream(token.string);
  double value = 0;
  if (!(stream >> value))
    throw runtime_error("invalid float");
  return shared_ptr<Expression>(new FloatValue(value));
}

shared_ptr<Expression> Parser::accept_fun_value
  (const shared_ptr<Environment> environment) {
  if (!accept(Token(Token::IDENTIFIER, "lam")))
    return shared_ptr<Expression>();
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
      parameters.push_back(parameter.string);
      if (!accept(Token::COMMA))
        break;
    }
  }
  expect(Token::RIGHT_PARENTHESIS);
  shared_ptr<Statement> body;
  if (accept(Token::COLON)) {
    auto expression(accept_expression(environment));
    if (!expression)
      expected("expression");
    body.reset(new RetStatement(expression));
  } else if (accept(Token::LEFT_BRACE)) {
    body = accept_statements(environment);
    expect(Token::RIGHT_BRACE);
  } else {
    expected("colon or block");
  }
  return shared_ptr<Expression>
    (new FunValue
     (identifier.string,
      parameters,
      body,
      environment));
}

shared_ptr<Expression> Parser::accept_integer_value
  (const shared_ptr<Environment> environment) {
  Token token;
  if (!accept(Token::INTEGER, token))
    return shared_ptr<Expression>();
  istringstream stream(token.string);
  int32_t value = 0;
  if (!(stream >> value))
    throw runtime_error("invalid integer");
  return shared_ptr<Expression>(new IntegerValue(value));
}

shared_ptr<Expression> Parser::accept_string_value
  (const shared_ptr<Environment>) {
  Token token;
  if (!accept(Token::STRING, token))
    return shared_ptr<Expression>();
  token.string.pop_back();
  return shared_ptr<Expression>(new StringValue(token.string.substr(1)));
}

shared_ptr<Expression> Parser::accept_undefined_value
  (const shared_ptr<Environment>) {
  if (accept(Token(Token::IDENTIFIER, "undefined")))
    return shared_ptr<Expression>(new UndefinedValue());
  return shared_ptr<Expression>();
}

}
