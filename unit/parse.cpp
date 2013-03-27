#include "BinaryExpression.h"
#include "BlockStatement.h"
#include "BooleanValue.h"
#include "CallExpression.h"
#include "ExpressionStatement.h"
#include "FlowStatement.h"
#include "IdentifierExpression.h"
#include "IntegerValue.h"
#include "Parser.h"
#include "SubscriptExpression.h"
#include "UndefinedValue.h"
#include "VarStatement.h"
#include "binary.h"
#include "tokenize.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace hap;
using namespace std;

#define TEST_PARSE(NAME, INPUT, EXPECTED) \
  test_parse \
    (__FILE__, __LINE__, \
     NAME, INPUT, unique_ptr<Statement>(EXPECTED))

void test_parse
  (const string& file,
   const int line,
   const string& name,
   const string& input,
   const unique_ptr<Statement> expected) {
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
    const auto expected(new BlockStatement());
    TEST_PARSE
      ("empty program",
       "",
       expected);
  }
  {
    const auto expected
      (new BlockStatement{new BlockStatement()});
    TEST_PARSE
      ("empty statement",
       ";",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new ExpressionStatement
        (new BooleanValue(true))});
    TEST_PARSE
      ("binary literal expression statement",
       "true;",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new ExpressionStatement
        (new IntegerValue(1234))});
    TEST_PARSE
      ("integer literal expression statement",
       "1234;",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new ExpressionStatement
        (new BinaryExpression
         (binary::operators["+"],
          new IntegerValue(12),
          new IntegerValue(34)))});
    TEST_PARSE
      ("simple arithmetic expression statement",
       "12+34;",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new ExpressionStatement
        (new CallExpression
         (new IdentifierExpression("f"),
          {}))});
    TEST_PARSE
      ("call expression statement with no arguments",
       "f();",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new ExpressionStatement
        (new CallExpression
         (new IdentifierExpression("f"),
          {new IdentifierExpression("x"),
           new IdentifierExpression("y")}))});
    TEST_PARSE
      ("call expression statement with arguments",
       "f(x, y);",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new ExpressionStatement
        (new SubscriptExpression
         (new CallExpression
          (new IdentifierExpression("f"),
           {new IdentifierExpression("x"),
            new IdentifierExpression("y")}),
          new IdentifierExpression("z")))});
    TEST_PARSE
      ("subscripted parenthesized call expression statement",
       "(f(x, y))[z];",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new ExpressionStatement
        (new CallExpression
         (new SubscriptExpression
          (new IdentifierExpression("f"),
           new IdentifierExpression("z")),
          {new IdentifierExpression("x"),
            new IdentifierExpression("y")}))});
    TEST_PARSE
      ("called subscript expression statement",
       "f[z](x, y);",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new ExpressionStatement
        (new SubscriptExpression
         (new IdentifierExpression("things"),
          new IdentifierExpression("index")))});
    TEST_PARSE
      ("subscript expression statement",
       "things[index];",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new VarStatement
        ("x",
         new UndefinedValue())});
    TEST_PARSE
      ("variable declaration without initializer",
       "var x;",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new VarStatement
        ("x",
         new IntegerValue(50))});
    TEST_PARSE
      ("variable declaration with literal initializer",
       "var x = 50;",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new VarStatement
        ("x",
         new BinaryExpression
          (binary::operators["+"],
           new IdentifierExpression("y"),
           new IdentifierExpression("z")))});
    TEST_PARSE
      ("variable declaration with expression initializer",
       "var x = y + z;",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new BlockStatement
        {new ExpressionStatement
         (new CallExpression
          (new IdentifierExpression("f"),
           {})),
        new ExpressionStatement
         (new CallExpression
          (new IdentifierExpression("g"),
           {})),
        new ExpressionStatement
         (new CallExpression
          (new IdentifierExpression("h"),
           {}))}});
    TEST_PARSE
      ("block statement with call expression statements",
       "{\n"
       "\tf();\n"
       "\tg();\n"
       "\th();\n"
       "}\n",
       expected);
  }
  {
    const auto expected
      (new BlockStatement
       {new IfStatement
        (new CallExpression
         (new IdentifierExpression("condition"),
          {new IdentifierExpression("x")}),
         new BlockStatement
          {new ExpressionStatement
           (new CallExpression
            (new IdentifierExpression("handler"),
             {new IdentifierExpression("x")}))})});
    TEST_PARSE
      ("variable declaration with expression initializer",
       "if (condition(x)) {\n"
       "\thandler(x);\n"
       "}\n",
       expected);
  }
}
