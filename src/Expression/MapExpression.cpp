#include "MapExpression.h"
#include "MapValue.h"
#include "unique_cast.h"

#include <ostream>

using namespace std;

namespace hap {

shared_ptr<Value> MapExpression::eval
  (const shared_ptr<Environment> environment) const {
  shared_ptr<MapValue> map(new MapValue());
  for (const auto& pair : pairs) {
    auto key(pair.first->eval(environment));
    auto value(pair.second->eval(environment));
    map->insert(key, value);
  }
  return static_pointer_cast<Value>(map);
}

void MapExpression::write(ostream& stream) const {
  stream << "{ ";
  for (const auto& pair : pairs) {
    pair.first->write(stream);
    stream << ": ";
    pair.second->write(stream);
    stream << ", ";
  }
  stream << '}';
}

}
