#include "MapExpression.h"
#include "MapValue.h"
#include "unique_cast.h"

#include <ostream>

using namespace std;

namespace hap {

unique_ptr<Value> MapExpression::eval(Environment& environment) const {
  unique_ptr<MapValue> map(new MapValue());
  for (const auto& pair : pairs) {
    auto key(pair.first->eval(environment));
    auto value(pair.second->eval(environment));
    map->insert(move(key), move(value));
  }
  return static_unique_cast<Value>(move(map));
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
