#ifndef HAP_MAPEXPRESSION_H
#define HAP_MAPEXPRESSION_H

#include "Expression.h"

#include <map>

namespace hap {

class MapExpression : public Expression {
public:
  MapExpression() {}
  void insert
    (std::shared_ptr<const Expression> key,
     std::shared_ptr<const Expression> value) {
    pairs.insert(std::make_pair(key, value));
  }
  virtual std::shared_ptr<Value> eval
    (Context&, const std::shared_ptr<Environment>) const final override;
  virtual void write(std::ostream&) const final override;
private:
  std::map
    <std::shared_ptr<const Expression>,
     std::shared_ptr<const Expression>> pairs;
};

}

#endif
