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
}
