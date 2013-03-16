#include "Parser.h"
#include "Statement.h"

using namespace std;

namespace hap {

unique_ptr<Statement>
Parser::accept_program() {
  unique_ptr<Statement> statements(accept_statements(global));
  if (!at_end())
    expected("end of input");
  return statements;
}

}
