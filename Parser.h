#ifndef HAP_PARSE_H
#define HAP_PARSE_H

#include "Token.h"

#include <memory>
#include <stack>
#include <vector>

namespace hap {

class Expression;
class Operator;
class Statement;

class Parser {
private:
  typedef std::unique_ptr<Statement> AcceptStatement();
  typedef std::unique_ptr<Expression> AcceptExpression();
  typedef bool AcceptOperator(Operator&);
public:
  Parser(const std::vector<Token>& input)
    : tokens(input), current(tokens.begin()) {}
  AcceptStatement accept_program;
private:
  AcceptStatement
    accept_statement,
    accept_statements,
    accept_empty_statement,
    accept_block_statement,
    accept_var_statement,
    accept_fun_statement,
    accept_ret_statement,
    accept_if_statement,
    accept_when_statement,
    accept_whenever_statement,
    accept_while_statement,
    accept_repeat_when_statement,
    accept_repeat_whenever_statement;
  AcceptExpression
    accept_expression,
    accept_value_expression,
    accept_boolean_expression,
    accept_integer_expression,
    accept_identifier_expression,
    accept_list_expression,
    accept_string_expression;
  AcceptOperator
    accept_binary_operator,
    accept_unary_operator;
  template<class T, class First, class... Rest>
  typename std::unique_ptr<T> first(First function, Rest&&... rest) {
    auto result((this->*function)());
    if (result)
      return std::move(result);
    return first<T, Rest...>(std::forward<Rest>(rest)...);
  }
  template<class T, class Only>
  typename std::unique_ptr<T> first(Only function) {
    return (this->*function)();
  }
  void E
    (std::stack<Operator>&,
     std::stack<std::unique_ptr<Expression>>&);
  void P
    (std::stack<Operator>&,
     std::stack<std::unique_ptr<Expression>>&);
  void push_operator
    (const Operator&,
     std::stack<Operator>&,
     std::stack<std::unique_ptr<Expression>>&);
  void pop_operator
    (std::stack<Operator>&,
     std::stack<std::unique_ptr<Expression>>&);
  typedef std::vector<Token> Tokens;
  const Tokens tokens;
  Tokens::const_iterator current;
  bool peek(Token::Type) const;
  bool peek(const Token&) const;
  bool accept(Token::Type);
  bool accept(Token::Type, Token&);
  bool accept(const Token&);
  void expect(Token::Type);
  void expect(Token::Type, Token&);
  void expect(const Token&);
  void expected(const std::string&) const;
  void expected(Token::Type) const;
  void expected(const Token&) const;
  bool at_end() const;
  template<class FlowStatement>
  std::unique_ptr<Statement> accept_flow_statement(const std::string&);
};

template<class StatementType>
std::unique_ptr<Statement>
Parser::accept_flow_statement(const std::string& keyword) {
  using namespace std;
  if (!accept(Token(Token::IDENTIFIER, keyword)))
    return unique_ptr<Statement>();
  unique_ptr<Expression> expression;
  expect(Token::LEFT_PARENTHESIS);
  if (!(expression = accept_expression()))
    expected("expression");
  expect(Token::RIGHT_PARENTHESIS);
  unique_ptr<Statement> statement;
  if (!(statement = accept_statement()))
    expected("statement");
  return unique_ptr<Statement>
    (new StatementType(move(expression), move(statement)));
}

}

#endif
