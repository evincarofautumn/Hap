#include "UndefinedExpression.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> UndefinedExpression::eval(Environment& environment) const {
  return unique_ptr<Value>(new UndefinedExpression(*this));
}

void UndefinedExpression::write(ostream& stream) const {
  stream << "undef";
}

}
