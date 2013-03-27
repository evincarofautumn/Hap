#ifndef HAP_PARSE_H
#define HAP_PARSE_H

#include "Environment.h"
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
  typedef std::shared_ptr<Expression> AcceptExpression
    (std::shared_ptr<Environment>);
  typedef bool AcceptOperator(Operator&);
  typedef std::shared_ptr<Statement> AcceptStatement
    (std::shared_ptr<Environment>);
  typedef std::shared_ptr<Expression> AcceptSuffix
    (std::shared_ptr<Environment>, std::shared_ptr<Expression>);
public:
  Parser(const std::vector<Token>&, std::shared_ptr<Environment>);
  std::shared_ptr<Statement> accept_program();
private:
  AcceptExpression
    accept_expression,
    accept_value_expression,
    // ----
    accept_boolean_expression,
    accept_identifier_expression,
    accept_integer_expression,
    accept_lambda_expression,
    accept_string_expression,
    accept_undefined_expression;
  AcceptOperator
    accept_binary_operator,
    accept_unary_operator;
  AcceptStatement
    accept_statement,
    accept_statements,
    // ----
    accept_atomic_statement,
    accept_block_statement,
    accept_empty_statement,
    accept_fun_statement,
    accept_if_statement,
    accept_last_statement,
    accept_next_statement,
    accept_ret_statement,
    accept_trace_statement,
    accept_var_statement,
    accept_when_statement,
    accept_whenever_statement,
    accept_while_statement,
    // ----
    accept_expression_statement;
  AcceptSuffix
    accept_suffix,
    // ----
    accept_call_suffix,
    accept_subscript_suffix;
  template<class T, class First, class... Rest>
  typename std::shared_ptr<T> first
    (const std::shared_ptr<Environment> environment,
     First function, Rest&&... rest) {
    auto result((this->*function)(environment));
    if (result)
      return result;
    return first<T, Rest...>(environment, std::forward<Rest>(rest)...);
  }
  template<class T, class Only>
  typename std::shared_ptr<T> first
    (const std::shared_ptr<Environment> environment, Only function) {
    return (this->*function)(environment);
  }
  void infix_expression
    (std::shared_ptr<Environment>,
     std::stack<Operator>&,
     std::stack<std::shared_ptr<Expression>>&);
  void infix_subexpression
    (std::shared_ptr<Environment>,
     std::stack<Operator>&,
     std::stack<std::shared_ptr<Expression>>&);
  void pop_operator
    (std::stack<Operator>&,
     std::stack<std::shared_ptr<Expression>>&);
  void push_operator
    (const Operator&,
     std::stack<Operator>&,
     std::stack<std::shared_ptr<Expression>>&);
  typedef std::vector<Token> Tokens;
  const Tokens tokens;
  Tokens::const_iterator current;
  void backtrack();
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
  std::shared_ptr<Statement>
  accept_flow_statement(std::shared_ptr<Environment>, const std::string&);
  std::shared_ptr<Environment> global;
};

template<class StatementType>
std::shared_ptr<Statement>
Parser::accept_flow_statement
  (const std::shared_ptr<Environment> environment,
   const std::string& keyword) {
  using namespace std;
  if (!accept(Token(Token::IDENTIFIER, keyword)))
    return shared_ptr<Statement>();
  shared_ptr<Expression> expression;
  expect(Token::LEFT_PARENTHESIS);
  if (!(expression = accept_expression(environment)))
    expected("expression");
  expect(Token::RIGHT_PARENTHESIS);
  shared_ptr<Statement> statement;
  if (!(statement = accept_statement(environment)))
    expected("statement");
  return shared_ptr<Statement>
    (new StatementType(expression, statement));
}

}

#endif
