#include "ListExpression.h"

#include "ListValue.h"
#include "indirect_equal.h"

#include <ostream>

using namespace std;

namespace hap {

shared_ptr<Value> ListExpression::eval
  (Context& context, const shared_ptr<Environment> environment) const {
  shared_ptr<ListValue> list(new ListValue());
  for (const auto& expression : expressions)
    list->push(expression->eval(context, environment));
  return static_pointer_cast<Value>(list);
}

bool ListExpression::equal(const Expression& expression) const {
  if (auto other
      = dynamic_cast<const ListExpression*>(&expression)) {
    return expressions.size() == other->expressions.size()
      && std::equal
        (begin(expressions), end(expressions),
         begin(other->expressions), indirect_equal<Expression>());
  }
  return false;
}

void ListExpression::write(ostream& stream) const {
  stream << "[ ";
  for (const auto& expression : expressions)
    stream << *expression << ", ";
  stream << ']';
}

}
