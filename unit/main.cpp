#include <iostream>
#include <stdexcept>

using namespace std;

void suite_tokenize();
void suite_parse();

int main(int argc, char** argv) try {
  suite_tokenize();
  suite_parse();
} catch (const exception& error) {
  cerr << error.what() << '\n';
  return 1;
}
