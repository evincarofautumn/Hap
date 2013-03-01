#include "tokenize.h"

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
  auto tokens = hap::tokenize(input);
  copy(tokens.begin(), tokens.end(),
       ostream_iterator<hap::Token>(cout, "\n"));
} catch (const exception& error) {
  cerr << error.what() << '\n';
  return 1;
}
