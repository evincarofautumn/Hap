#ifndef HAP_PARSE_H
#define HAP_PARSE_H

#include "Token.h"

#include <stack>
#include <tr1/memory>
#include <vector>

namespace hap {

class Expression;
class Operator;
class Statement;

class Parser {
private:
  typedef std::tr1::shared_ptr<Statement> AcceptStatement();
  typedef std::tr1::shared_ptr<Expression> AcceptExpression();
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
    accept_if_statement,
    accept_when_statement,
    accept_whenever_statement,
    accept_while_statement,
    accept_repeat_when_statement,
    accept_repeat_whenever_statement;
  AcceptExpression
    accept_expression,
    accept_value_expression,
    accept_integer_expression,
    accept_identifier_expression,
    accept_list_expression;
  AcceptOperator
    accept_binary_operator,
    accept_unary_operator;

  void E
    (std::stack<Operator>&,
     std::stack< std::tr1::shared_ptr<Expression> >&);
  void P
    (std::stack<Operator>&,
     std::stack< std::tr1::shared_ptr<Expression> >&);
  void pushOperator
    (const Operator&,
     std::stack<Operator>&,
     std::stack< std::tr1::shared_ptr<Expression> >&);
  void popOperator
    (std::stack<Operator>&,
     std::stack< std::tr1::shared_ptr<Expression> >&);

  typedef std::vector<Token> Tokens;
  const Tokens tokens;
  Tokens::const_iterator current;
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
  std::tr1::shared_ptr<Statement> accept_flow_statement(const std::string&);
};

template<class StatementType>
std::tr1::shared_ptr<Statement>
Parser::accept_flow_statement(const std::string& keyword) {
  using namespace std::tr1;
  if (!accept(Token(Token::IDENTIFIER, keyword)))
    return shared_ptr<Statement>();
  shared_ptr<Expression> expression;
  expect(Token::LEFT_PARENTHESIS);
  if (!(expression = accept_expression()))
    expected("expression");
  expect(Token::RIGHT_PARENTHESIS);
  shared_ptr<Statement> statement;
  if (!(statement = accept_statement()))
    expected("statement");
  return shared_ptr<Statement>(new StatementType(expression, statement));
}

}

#endif
