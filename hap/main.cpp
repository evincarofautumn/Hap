#include "Interpreter.h"
#include "Parser.h"
#include "Statement.h"
#include "tokenize.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>

using namespace hap;
using namespace std;

int main(int argc, char** argv) try {
  --argc;
  ++argv;
  if (argc != 1)
    throw runtime_error("Bad command line.");
  ifstream input(argv[0]);
  const auto global = make_shared<Environment>();
  Interpreter(global).run
    (Parser(tokenize(input), global).accept_program());
} catch (const exception& error) {
  cerr << error.what() << '\n';
  return 1;
}
