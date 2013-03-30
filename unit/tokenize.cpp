#include "tokenize.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace hap;
using namespace std;

#define TEST_TOKENIZE(NAME, INPUT, EXPECTED) \
  test_tokenize(__FILE__, __LINE__, NAME, INPUT, EXPECTED)

void test_tokenize
  (const string& file,
   const int line,
   const string& name,
   const string& input,
   const vector<Token>& expected) {
  istringstream stream(input);
  const auto actual(tokenize(stream));
  if (actual == expected)
    return;
  ostringstream message;
  message
    << file << ":" << line
    << ": Unit test '" << name << "' failed:\n"
    << "\texpected: ";
  copy(begin(expected), end(expected),
    ostream_iterator<Token>(message, " "));
  message << "\n\t  actual: ";
  copy(begin(actual), end(actual),
    ostream_iterator<Token>(message, " "));
  message << '\n';
  throw runtime_error(message.str());
}

void suite_tokenize() {
  {
    vector<Token> expected;
    TEST_TOKENIZE
      ("empty input",
       "",
       expected);
  }

  {
    vector<Token> expected;
    TEST_TOKENIZE
      ("only spaces",
       "     ",
       expected);
  }

  {
    vector<Token> expected;
    TEST_TOKENIZE
      ("all whitespace characters",
       " \t\n\r\f\v",
       expected);
  }

  {
    vector<Token> expected;
    TEST_TOKENIZE
      ("comment ending at end of input",
       "# this is a comment",
       expected);
  }

  {
    vector<Token> expected;
    TEST_TOKENIZE
      ("comment with newline",
       "# this is a comment\n",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::LEFT_PARENTHESIS, "("));
    TEST_TOKENIZE
      ("left parenthesis",
       "(",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::RIGHT_PARENTHESIS, ")"));
    TEST_TOKENIZE
      ("right parenthesis",
       ")",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::LEFT_BRACE, "{"));
    TEST_TOKENIZE
      ("left brace",
       "{",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::RIGHT_BRACE, "}"));
    TEST_TOKENIZE
      ("right brace",
       "}",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::LEFT_BRACKET, "["));
    TEST_TOKENIZE
      ("left bracket",
       "[",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::RIGHT_BRACKET, "]"));
    TEST_TOKENIZE
      ("right bracket",
       "]",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::COMMA, ","));
    TEST_TOKENIZE
      ("comma",
       ",",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::SEMICOLON, ";"));
    TEST_TOKENIZE
      ("semicolon",
       ";",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::COLON, ":"));
    TEST_TOKENIZE
      ("colon",
       ":",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::STRING, "\"\""));
    TEST_TOKENIZE
      ("empty string literal",
       "\"\"",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::STRING, "\"test\""));
    TEST_TOKENIZE
      ("string literal",
       "\"test\"",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::STRING, "\"\a\b\f\n\r\t\v\"\\\""));
    TEST_TOKENIZE
      ("string literal with escapes",
       "\"\\a\\b\\f\\n\\r\\t\\v\\\"\\\\\"",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::INTEGER, "0"));
    TEST_TOKENIZE
      ("zero",
       "0",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::INTEGER, "1"));
    TEST_TOKENIZE
      ("single-digit integer",
       "1",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::INTEGER, "12345"));
    TEST_TOKENIZE
      ("multiple-digit integer",
       "12345",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "_"));
    TEST_TOKENIZE
      ("underscore",
       "_",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "abc"));
    TEST_TOKENIZE
      ("lowercase alphabetic identifier",
       "abc",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "abc123"));
    TEST_TOKENIZE
      ("lowercase alphanumeric identifier",
       "abc123",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "Abc123"));
    TEST_TOKENIZE
      ("mixed-case alphanumeric identifier",
       "Abc123",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "abc_123"));
    TEST_TOKENIZE
      ("alphanumeric identifier with underscores",
       "abc_123",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "abc"));
    expected.push_back(Token(Token::IDENTIFIER, "_123"));
    expected.push_back(Token(Token::IDENTIFIER, "abc_123"));
    expected.push_back(Token(Token::IDENTIFIER, "Abc123"));
    expected.push_back(Token(Token::IDENTIFIER, "ABC"));
    TEST_TOKENIZE
      ("whitespace-separated identifiers",
       "abc _123\tabc_123\n Abc123\n\tABC",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::OPERATOR, "+"));
    TEST_TOKENIZE
      ("single-character operator",
       "+",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::OPERATOR, "!!!"));
    TEST_TOKENIZE
      ("multi-character operator",
       "!!!",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::OPERATOR, "!$%&*+-./<=>?@\\^|~"));
    TEST_TOKENIZE
      ("all operator characters",
       "!$%&*+-./<=>?@\\^|~",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::OPERATOR, "+"));
    expected.push_back(Token(Token::COMMA, ","));
    expected.push_back(Token(Token::OPERATOR, "-"));
    expected.push_back(Token(Token::SEMICOLON, ";"));
    expected.push_back(Token(Token::DOT, "."));
    expected.push_back(Token(Token::DOT, "."));
    expected.push_back(Token(Token::DOT, "."));
    expected.push_back(Token(Token::COLON, ":"));
    expected.push_back(Token(Token::OPERATOR, "?"));
    TEST_TOKENIZE
      ("mixed operators and single-character tokens",
       "+,-;...:?",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "abc"));
    expected.push_back(Token(Token::STRING, "\"abc\""));
    TEST_TOKENIZE
      ("identifier followed by string literal",
       "abc\"abc\"",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::STRING, "\"abc\""));
    expected.push_back(Token(Token::IDENTIFIER, "abc"));
    TEST_TOKENIZE
      ("string literal followed by identifier",
       "\"abc\"abc",
       expected);
  }

  {
    vector<Token> expected;
    expected.push_back(Token(Token::INTEGER, "123"));
    expected.push_back(Token(Token::IDENTIFIER, "abc"));
    TEST_TOKENIZE
      ("integer followed by identifier",
       "123abc",
       expected);
  }
}
