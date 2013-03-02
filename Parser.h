#ifndef HAP_PARSE_H
#define HAP_PARSE_H

#include "Token.h"

#include <vector>

namespace hap {

class Expression;
class Statement;

class Parser {
private:
  typedef Statement* AcceptStatement();
  typedef Expression* AcceptExpression();
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
    accept_identifier_expression;
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
  Statement* accept_flow_statement(const std::string&);
};

template<class StatementType>
Statement* Parser::accept_flow_statement(const std::string& keyword) {
  if (!accept(Token(Token::IDENTIFIER, keyword)))
    return 0;
  Expression* expression = 0;
  expect(Token::LEFT_PARENTHESIS);
  if (!(expression = accept_expression()))
    expected("expression");
  expect(Token::RIGHT_PARENTHESIS);
  Statement* statement = 0;
  if (!(statement = accept_statement()))
    expected("statement");
  return new StatementType(expression, statement);
}

}

#endif
