#include "tokenize.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace hap;
using namespace std;

void suite(const string&);
void suite_tokenize();

int main(int argc, char** argv) try {
  suite_tokenize();
} catch (const exception& error) {
  cerr << error.what() << '\n';
  return 1;
}

void test_tokenize
  (const string& name,
   const string& input,
   const vector<Token>& expected) {
  istringstream stream(input);
  const auto actual(tokenize(stream));
  if (actual == expected) {
    cout << "Unit test '" << name << "' passed.\n";
    return;
  }
  ostringstream message;
  message
    << "Unit test '" << name << "' failed:\n"
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
    test_tokenize
      ("empty input",
       "",
       expected);
  }
  {
    vector<Token> expected;
    test_tokenize
      ("only spaces",
       "     ",
       expected);
  }
  {
    vector<Token> expected;
    test_tokenize
      ("all whitespace characters",
       " \t\n\r\f\v",
       expected);
  }
  {
    vector<Token> expected;
    test_tokenize
      ("comment ending at end of input",
       "# this is a comment",
       expected);
  }
  {
    vector<Token> expected;
    test_tokenize
      ("comment with newline",
       "# this is a comment\n",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::LEFT_PARENTHESIS, "("));
    test_tokenize
      ("left parenthesis",
       "(",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::RIGHT_PARENTHESIS, ")"));
    test_tokenize
      ("right parenthesis",
       ")",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::LEFT_BRACE, "{"));
    test_tokenize
      ("left brace",
       "{",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::RIGHT_BRACE, "}"));
    test_tokenize
      ("right brace",
       "}",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::LEFT_BRACKET, "["));
    test_tokenize
      ("left bracket",
       "[",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::RIGHT_BRACKET, "]"));
    test_tokenize
      ("right bracket",
       "]",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::COMMA, ","));
    test_tokenize
      ("comma",
       ",",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::SEMICOLON, ";"));
    test_tokenize
      ("semicolon",
       ";",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::COLON, ":"));
    test_tokenize
      ("colon",
       ":",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::STRING, "\"\""));
    test_tokenize
      ("empty string literal",
       "\"\"",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::STRING, "\"test\""));
    test_tokenize
      ("string literal",
       "\"test\"",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::INTEGER, "0"));
    test_tokenize
      ("zero",
       "0",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::INTEGER, "1"));
    test_tokenize
      ("single-digit integer",
       "1",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::INTEGER, "12345"));
    test_tokenize
      ("multiple-digit integer",
       "12345",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "_"));
    test_tokenize
      ("underscore",
       "_",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "abc"));
    test_tokenize
      ("lowercase alphabetic identifier",
       "abc",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "abc123"));
    test_tokenize
      ("lowercase alphanumeric identifier",
       "abc123",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "Abc123"));
    test_tokenize
      ("mixed-case alphanumeric identifier",
       "Abc123",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::IDENTIFIER, "abc_123"));
    test_tokenize
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
    test_tokenize
      ("whitespace-separated identifiers",
       "abc _123\tabc_123\n Abc123\n\tABC",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::OPERATOR, "+"));
    test_tokenize
      ("single-character operator",
       "+",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::OPERATOR, "!!!"));
    test_tokenize
      ("multi-character operator",
       "!!!",
       expected);
  }
  {
    vector<Token> expected;
    expected.push_back(Token(Token::OPERATOR, "!$%&*+-./<=>?@\\^|~"));
    test_tokenize
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
    expected.push_back(Token(Token::OPERATOR, "..."));
    expected.push_back(Token(Token::COLON, ":"));
    expected.push_back(Token(Token::OPERATOR, "?"));
    test_tokenize
      ("mixed operators and single-character tokens",
       "+,-;...:?",
       expected);
  }
}
