#include "ListExpression.h"
#include "ListValue.h"
#include "unique_cast.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> ListExpression::eval(Environment& environment) const {
  unique_ptr<ListValue> list(new ListValue());
  for (const auto& expression : expressions)
    list->push(expression->eval(environment));
  return static_unique_cast<Value>(move(list));
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
