#include "tokenize.h"
#include "Parser.h"
#include "Statement.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace std;

int main(int argc, char** argv) try {
  --argc;
  ++argv;
  if (argc != 1)
    throw runtime_error("Bad command line.");
  ifstream input(argv[0]);
  vector<hap::Token> tokens = hap::tokenize(input);
  hap::Parser parser(tokens);
  hap::Statement* program = parser.accept_program();
  delete program;
} catch (const exception& error) {
  cerr << error.what() << '\n';
  return 1;
}
