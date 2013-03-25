#include "BlockStatement.h"
#include "Parser.h"
#include "tokenize.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace hap;
using namespace std;

#define TEST_PARSE(NAME, INPUT, EXPECTED) \
  test_parse(__FILE__, __LINE__, NAME, INPUT, EXPECTED)

void test_parse
  (const string& file,
   const int line,
   const string& name,
   const string& input,
   const shared_ptr<Statement> expected) {
  shared_ptr<Statement> actual;
  {
    const shared_ptr<Environment> environment(new Environment());
    istringstream stream(input);
    const auto tokens(tokenize(stream));
    Parser parser(tokens, environment);
    actual = parser.accept_program();
  }
  if (*actual == *expected)
    return;
  ostringstream message;
  message
    << file << ":" << line
    << ": Unit test '" << name << "' failed:\n"
    << "Expected:\n" << *expected
    << "Actual:\n" << *actual
    << '\n';
  throw runtime_error(message.str());
}

void suite_parse() {
  {
    shared_ptr<Statement> expected(new BlockStatement());
    TEST_PARSE
      ("empty program",
       "",
       expected);
  }
  {
    shared_ptr<BlockStatement> expected(new BlockStatement());
    expected->push(shared_ptr<Statement>(new BlockStatement()));
    TEST_PARSE
      ("empty statement",
       ";",
       expected);
  }
}
