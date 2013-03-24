#include "Parser.h"
#include "Statement.h"

using namespace std;

namespace hap {

Parser::Parser
  (const vector<Token>& input,
   const shared_ptr<Environment> global)
  : tokens(input),
    current(tokens.begin()),
    global(global) {}

shared_ptr<Statement>
Parser::accept_program() {
  shared_ptr<Statement> statements(accept_statements(global));
  if (!at_end())
    expected("end of input");
  return statements;
}

}
