#include "ListExpression.h"
#include "ListValue.h"
#include "unique_cast.h"

#include <ostream>

using namespace std;

namespace hap {

shared_ptr<Value> ListExpression::eval
  (const shared_ptr<Environment> environment) const {
  shared_ptr<ListValue> list(new ListValue());
  for (const auto& expression : expressions)
    list->push(expression->eval(environment));
  return static_pointer_cast<Value>(list);
}

void ListExpression::write(ostream& stream) const {
  stream << "[ ";
  for (const auto& expression : expressions) {
    expression->write(stream);
    stream << ", ";
  }
  stream << ']';
}

}
