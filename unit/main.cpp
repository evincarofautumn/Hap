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
}
