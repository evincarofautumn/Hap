#include <iostream>
#include <stdexcept>

using namespace std;

void suite_tokenize();

int main(int argc, char** argv) try {
  suite_tokenize();
} catch (const exception& error) {
  cerr << error.what() << '\n';
  return 1;
}
