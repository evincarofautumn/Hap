#include "Parser.h"

#include "BinaryExpression.h"
#include "CallExpression.h"
#include "DotExpression.h"
#include "IdentifierExpression.h"
#include "ListExpression.h"
#include "MapExpression.h"
#include "Operator.h"
#include "Operator.h"
#include "RetStatement.h"
#include "StringValue.h"
#include "SubscriptExpression.h"
#include "UnaryExpression.h"
#include "binary.h"
#include "unary.h"

using namespace std;

namespace hap {

struct not_an_expression : public runtime_error {
  not_an_expression() : runtime_error("not an expression") {}
};

shared_ptr<Expression> Parser::accept_expression
  (const shared_ptr<Environment> environment) {
  try {
    stack<Operator> operators;
    stack<shared_ptr<Expression>> operands;
    operators.push(Operator());
    infix_expression(environment, operators, operands);
    return operands.top();
  } catch (const not_an_expression&) {
    return nullptr;
  }
}

shared_ptr<Expression>
Parser::accept_value_expression(const shared_ptr<Environment> environment) {
  auto value(first<Expression>
    (environment,
     &Parser::accept_boolean_value,
     &Parser::accept_float_value,
     &Parser::accept_fun_value,
     &Parser::accept_integer_value,
     &Parser::accept_string_value,
     &Parser::accept_undefined_value,
     &Parser::accept_identifier_expression));
  if (!value)
    return nullptr;
  return accept_suffix(environment, value);
}

shared_ptr<Expression> Parser::accept_suffix
  (const shared_ptr<Environment> environment,
   shared_ptr<Expression> current) {
  decltype(current) previous;
  do {
    previous = current;
    current = accept_dot_suffix(environment, current);
    current = accept_call_suffix(environment, current);
    current = accept_subscript_suffix(environment, current);
  } while (current != previous);
  return current;
}

shared_ptr<Expression> Parser::accept_dot_suffix
  (const shared_ptr<Environment> environment,
   shared_ptr<Expression> value) {
  if (!accept(Token::DOT))
    return value;
  Token token;
  if (!accept(Token::IDENTIFIER, token))
    expected("identifier");
  return make_shared<DotExpression>(value, token.string);
}

shared_ptr<Expression> Parser::accept_call_suffix
  (const shared_ptr<Environment> environment,
   shared_ptr<Expression> value) {
  if (!accept(Token::LEFT_PARENTHESIS))
    return value;
  vector<shared_ptr<Expression>> arguments;
  if (!peek(Token::RIGHT_PARENTHESIS)) {
    while (true) {
      if (peek(Token::RIGHT_PARENTHESIS))
        break;
      auto argument(accept_expression(environment));
      if (!argument)
        expected("expression");
      arguments.push_back(argument);
      if (!accept(Token::COMMA))
        break;
    }
  }
  expect(Token::RIGHT_PARENTHESIS);
  return make_shared<CallExpression>(value, move(arguments));
}

shared_ptr<Expression>
Parser::accept_subscript_suffix
  (const shared_ptr<Environment> environment,
   shared_ptr<Expression> value) {
  if (!accept(Token::LEFT_BRACKET))
    return value;
  const auto index(accept_expression(environment));
  if (!index)
    expected("expression");
  expect(Token::RIGHT_BRACKET);
  return make_shared<SubscriptExpression>(value, index);
}

shared_ptr<Expression>
Parser::accept_identifier_expression(const shared_ptr<Environment>) {
  Token token;
  if (!accept(Token::IDENTIFIER, token))
    return nullptr;
  return make_shared<IdentifierExpression>(token.string);
}

bool Parser::accept_binary_operator(Operator& result) {
  if (at_end() || !(current->type == Token::OPERATOR
      || current->type == Token::IDENTIFIER))
    return false;
  map<string, Operator>::const_iterator operator_
    (binary::operators.find(current->string));
  if (operator_ == binary::operators.end())
    return false;
  ++current;
  result = operator_->second;
  return true;
}

bool Parser::accept_unary_operator(Operator& result) {
  if (at_end() || !(current->type == Token::OPERATOR
      || current->type == Token::IDENTIFIER))
    return false;
  map<string, Operator>::const_iterator operator_
    (unary::operators.find(current->string));
  if (operator_ == unary::operators.end())
    return false;
  ++current;
  result = operator_->second;
  return true;
}

void Parser::infix_expression
  (const shared_ptr<Environment> environment,
   stack<Operator>& operators,
   stack<shared_ptr<Expression>>& operands) {
  infix_subexpression(environment, operators, operands);
  Operator operator_;
  while (accept_binary_operator(operator_)) {
    push_operator(operator_, operators, operands);
    infix_subexpression(environment, operators, operands);
  }
  while (operators.top().arity != Operator::SENTINEL)
    pop_operator(operators, operands);
}

void Parser::infix_subexpression
  (const shared_ptr<Environment> environment,
   stack<Operator>& operators,
   stack<shared_ptr<Expression>>& operands) {
  Operator unary;
  shared_ptr<Expression> value;
  if (accept(Token::LEFT_PARENTHESIS)) {
    operators.push(Operator());
    infix_expression(environment, operators, operands);
    expect(Token::RIGHT_PARENTHESIS);
    operators.pop();
    auto value(operands.top());
    operands.pop();
    operands.push(accept_suffix(environment, value));
  } else if (accept(Token::LEFT_BRACKET)) {
    const auto list = make_shared<ListExpression>();
    operators.push(Operator());
    if (!peek(Token::RIGHT_BRACKET)) {
      while (true) {
        if (peek(Token::RIGHT_BRACKET))
          break;
        infix_expression(environment, operators, operands);
        list->push(operands.top());
        operands.pop();
        if (!accept(Token::COMMA))
          break;
      }
    }
    expect(Token::RIGHT_BRACKET);
    operators.pop();
    operands.push(static_pointer_cast<Expression>(list));
  } else if (accept(Token::LEFT_BRACE)) {
    const auto map = make_shared<MapExpression>();
    operators.push(Operator());
    if (!peek(Token::RIGHT_BRACE)) {
      while (true) {
        if (peek(Token::RIGHT_BRACE))
          break;
        shared_ptr<const Expression> key;
        Token bareword;
        if (accept(Token::IDENTIFIER, bareword)) {
          key.reset(new StringValue(bareword.string));
        } else {
          infix_expression(environment, operators, operands);
          key = operands.top();
          operands.pop();
        }
        expect(Token::COLON);
        infix_expression(environment, operators, operands);
        shared_ptr<const Expression> value(operands.top());
        operands.pop();
        map->insert(key, value);
        if (!accept(Token::COMMA))
          break;
      }
    }
    expect(Token::RIGHT_BRACE);
    operators.pop();
    operands.push(static_pointer_cast<Expression>(map));
  } else if (accept_unary_operator(unary)) {
    push_operator(unary, operators, operands);
    infix_subexpression(environment, operators, operands);
  } else if ((value = accept_value_expression(environment))) {
    operands.push(value);
  } else {
    throw not_an_expression();
  }
}

void Parser::pop_operator
  (stack<Operator>& operators,
   stack<shared_ptr<Expression>>& operands) {
  if (operators.top().arity == Operator::BINARY) {
    Operator operator_(operators.top());
    operators.pop();
    shared_ptr<Expression> b(operands.top());
    operands.pop();
    shared_ptr<Expression> a(operands.top());
    operands.pop();
    operands.push(make_shared<BinaryExpression>(operator_, a, b));
  } else if (operators.top().arity == Operator::UNARY) {
    Operator operator_(operators.top());
    operators.pop();
    shared_ptr<Expression> a(operands.top());
    operands.pop();
    operands.push(make_shared<UnaryExpression>(operator_, a));
  } else {
    throw runtime_error("error parsing expression");
  }
}

void Parser::push_operator
  (const Operator& operator_,
   stack<Operator>& operators,
   stack<shared_ptr<Expression>>& operands) {
  while (operator_ < operators.top())
    pop_operator(operators, operands);
  operators.push(operator_);
}

}
