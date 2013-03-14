#include "Statement.h"
#include "Environment.h"
#include "Expression.h"
#include "flow.h"
#include "unique_cast.h"

#include <ostream>

using namespace std;

namespace hap {

Statement::~Statement() {}

ostream& operator<<(ostream& stream, const Statement& statement) {
  statement.write(stream);
  return stream;
}

}
