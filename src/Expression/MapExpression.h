#ifndef HAP_MAPEXPRESSION_H
#define HAP_MAPEXPRESSION_H

#include "Expression.h"

#include <map>

namespace hap {

class MapExpression : public Expression {
public:
  MapExpression() {}
  void insert(std::unique_ptr<const Expression> key,
              std::unique_ptr<const Expression> value) {
    pairs.insert(std::make_pair(std::move(key), std::move(value)));
  }
  virtual std::unique_ptr<Value> eval(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::map
    <std::unique_ptr<const Expression>,
     std::unique_ptr<const Expression>> pairs;
};

}

#endif
