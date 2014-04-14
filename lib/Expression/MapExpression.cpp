#include "MapExpression.h"

#include "MapValue.h"
#include "indirect_compare.h"

#include <ostream>

using namespace std;

namespace hap {

shared_ptr<Value> MapExpression::eval
  (Context& context, const shared_ptr<Environment> environment) const {
  const auto map = make_shared<MapValue>();
  for (const auto& pair : pairs) {
    auto key(pair.first->eval(context, environment));
    auto value(pair.second->eval(context, environment));
    map->insert(key, value);
  }
  return static_pointer_cast<Value>(map);
}

bool MapExpression::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const MapExpression*>(&expression)) {
    return pairs.size() == that->pairs.size()
      && std::equal
        (begin(pairs), end(pairs),
         begin(that->pairs),
         pair_indirect_equal<Expression, Expression>());
  }
  return false;
}

void MapExpression::write(ostream& stream) const {
  stream << "{ ";
  for (const auto& pair : pairs)
    stream << *pair.first << ": " << *pair.second << ", ";
  stream << '}';
}

}
